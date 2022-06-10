#include "LSSSDKW21.h"
#include "BinaryTreeNode.h"
#include "Eigen/src/Core/Matrix.h"
#include <math.h>
#include <queue>
#include <string>

AccessControlParameter *LSSSDKW21::generateAccessControl(const std::vector<std::vector<int>> &accessPolicy, 
  const std::vector<std::string> &rhos) {
    // init access tree
    AccessTreeNode *rootAccessTreeNode = AccessTreeNode::GenerateAccessTree(accessPolicy, rhos);
    // reconstruct binary tree node
    BinaryTreeNode *rootBinaryTreeNode = BinaryTreeNode::ReconstructBinaryTreeNode(accessPolicy, rhos);

    // generate lsss matrix
    std::unordered_map<std::string, std::list<std::vector<int>>> map;
    int maxLen = 0;
    int rows = 0;
    // We maintain a global counter variable c, which is initialized to 1.
    int c = 2;
    int k = rhos.size() - 1;
    std::vector<int> vector1(k+2, 0);
    std::vector<int> vector0(k+2, 0);
    vector1.at(0) = 1;
    vector0.at(1) = 1;
    rootBinaryTreeNode->setVector1(vector1);
    rootBinaryTreeNode->setVector0(vector0);

    std::queue<BinaryTreeNode*> queue;
    queue.push(rootBinaryTreeNode);

    while (!queue.empty()) {
        BinaryTreeNode *p = queue.front();
        queue.pop();
        std::vector<int> w1 = p->getVector1();
        std::vector<int> w0 = p->getVector0();
        if (p->getType() == BinaryTreeNode::NodeType::AND) {
            // If the parent node is AND gate labeled by the vector v
            // We pad v with 0's at the end (if necessary) to make it of length c.
            // Then we label one of its children (right children) with the vector v|1
            BinaryTreeNode *left = p->getLeft();
            std::vector<int> u1(k+2, 0);
            u1.at(c) = 1;
            std::vector<int> u0(w0);
            left->setVector1(u1);
            left->setVector0(u0);
            queue.push(left);

            // Then we label one of its children (left children) with the vector (0,...,0)|-1
            BinaryTreeNode *right = p->getRight();
            std::vector<int> v1(k+2, 0);
            std::vector<int> v0(k+2, 0);
            for (int i = 0; i < k+2; i++) {
                v1.at(i) = w1.at(i)-u1.at(i);
                v0.at(i) = w0.at(i)-u1.at(i);
            }
            right->setVector1(v1);
            right->setVector0(v0);
            queue.push(right);
            // We now increment the value of c by 1.
            c += 1;
        } else if (p->getType() == BinaryTreeNode::NodeType::OR) {
            //If the parent node is an OR gate labeled by the vector v
            BinaryTreeNode *left = p->getLeft();
            std::vector<int> u1(w1);
            std::vector<int> u0(k+2, 0);
            u0.at(c) = 1;
            //then we also label its (left) children by v (and the value of c stays the same)
            // lv.assign(p->getVector());
            left->setVector1(u1);
            left->setVector0(u0);
            queue.push(left);

            BinaryTreeNode *right = p->getRight();
            std::vector<int> v1(k+2, 0);
            std::vector<int> v0(k+2, 0);
            //then we also label its (right) children by v (and the value of c stays the same)
            for (int i=0; i<k+2; ++i) {
                v1.at(i) = w1.at(i) - u0.at(i);
                v0.at(i) = w0.at(i) - u0.at(i);
            }
            right->setVector1(v1);
            right->setVector0(v0);
            queue.push(right);
            c += 1;
        } else {
            // leaf node
            rows += 2;
            if (map.find(p->getValue()) != map.end()) {
                map.at(p->getValue()).push_back(p->getVector1());
                map.at(p->getValue()).push_back(p->getVector0());
            } else {
                std::list<std::vector<int>> list;
                list.push_back(p->getVector1());
                list.push_back(p->getVector0());
                map[p->getValue()] = list;
            }
        }
    }

    // Construct the lsss Matrix
    std::vector<std::vector<int>> lsssMatrix(rows);
    std::vector<std::string> rhosParameter(rhos.size());
    int i = 0, j = 0;
    for (auto &pair : map) {
        std::list<std::vector<int>> v = pair.second;
        rhosParameter.at(i++) = pair.first;
        for (std::vector<int> aV : v) {
            lsssMatrix.at(j++) = aV;
        }
    }
    return new LSSSPolicyParameter(rootAccessTreeNode, accessPolicy, lsssMatrix, rhosParameter);
}

std::unordered_map<std::string, element_t> LSSSDKW21::secretSharing(pairing_t pairing, element_t secret, 
  AccessControlParameter *accessControlParameter) {
    LSSSPolicyParameter *lsssPolicyParameter = static_cast<LSSSPolicyParameter*>(accessControlParameter);
    int row = lsssPolicyParameter->getRow();
    int column = lsssPolicyParameter->getColumn();
    std::vector<std::vector<int>> lsssMatrix = lsssPolicyParameter->getLSSSMatrix();
    // std::vector<std::vector<element_t>> elementLSSSMatrix(row, std::vector<element_t>(column));
    element_t **elementLSSSMatrix = new element_t*[row];
    for (int i = 0; i < lsssPolicyParameter->getRow(); i++) {
        elementLSSSMatrix[i] = new element_t[column];
        for (int j = 0; j < lsssPolicyParameter->getColumn(); j++) {
            element_init_Zr(elementLSSSMatrix[i][j], pairing);
            element_set_si(elementLSSSMatrix[i][j], lsssMatrix[i][j]);
            // elementLSSSMatrix[i][j] = pairing.getZr().newElement(lsssMatrix[i][j]).getImmutable();
        }
    }
    // Init vector v
    std::vector<element_t> elementsV(column);
    element_init_same_as(elementsV[0], secret);
    element_set(elementsV[0], secret);
    assert(element_cmp(elementsV[0], secret) == 0); // assert elementsV[0] == secret
    for (int i = 1; i < elementsV.size(); i++) {
        element_init_Zr(elementsV[i], pairing);
        element_random(elementsV[i]);
    }
    // Secret share by matrix multiplication
    std::unordered_map<std::string, element_t> lambdaElementsMap;
    std::vector<std::string> rhos = lsssPolicyParameter->getRhos();
    for (int i=0; i<row; i++) {
        element_t elementsLambda;
        element_init_Zr(elementsLambda, pairing);
        element_set0(elementsLambda);
        for (int j=0; j<column; j++) {
            element_t e;
            element_init_same_as(e, elementsV[j]);
            element_mul_zn(e, elementLSSSMatrix[i][j], elementsV[j]);
            element_add(elementsLambda, elementsLambda, e);
        }
        element_init_same_as(lambdaElementsMap[rhos.at(i/2)+'_'+std::to_string((i+1)%2)], elementsLambda);
        element_set(lambdaElementsMap[rhos.at(i/2)+'_'+std::to_string((i+1)%2)], elementsLambda);
    }
    return lambdaElementsMap;
}

std::unordered_map<std::string, element_t> LSSSDKW21::reconstructOmegas(pairing_t pairing, 
  std::vector<std::string> attributes, AccessControlParameter *accessControlParameter) {
    LSSSPolicyParameter *lsssPolicyParameter = static_cast<LSSSPolicyParameter*>(accessControlParameter);
    
    std::vector<std::string> minSatisfiedAttributes = lsssPolicyParameter->minSatisfiedAttributeSet(attributes);
    std::vector<std::string> leafAttributes = lsssPolicyParameter->getRhos();
    std::vector<int> rows(leafAttributes.size());
    int counter = 0;
    for (int i = 0; i < leafAttributes.size(); i++) {
        bool findit = false;
        for (std::string minSatisfiedAttribute : minSatisfiedAttributes) {
            if (leafAttributes[i] == minSatisfiedAttribute) {
                //比较L矩阵和获得的S参数中各个元素，记下所有相同的元素对应的在数组中的位置，并生成一个新的矩阵，把相同的元素存在一个叫做result的数组之中，长度为counter
                rows[counter++] = 2 * i;
                findit = true;
                break;
            }
        }
        if (!findit) {rows[counter++] = 2 * i + 1;}
    }
    assert(counter == leafAttributes.size());
    assert(counter + 1 == lsssPolicyParameter->getColumn());

    std::vector<int> result(rows);
    Eigen::MatrixXd mA(counter, counter);
    std::vector<int> cols(rows.size());
    for(int j = 0; j < lsssPolicyParameter->getColumn(); j++){
        counter = 0;
        for (int k = 0; k < lsssPolicyParameter->getColumn(); ++k) {
            if (k != j) {
                cols[counter++] = k;
            }
        }
        assert(counter == result.size());
        for (int i = 0; i < counter; ++i) {
            for (int j = 0; j < counter; ++j) {
                mA(j, i) = lsssPolicyParameter->getLSSSMatrix()[result[j]][cols[i]];
            }
        }
        if (mA.determinant() != 0) {
            break;
        }
    }
    assert(mA.determinant() != 0);
    //solve the linear system
    // Eigen::MatrixXd mA(m);
    // for(int i = 0; i < result.size(); i++){
    //     for(int j = 0; j < result.size(); j++){
    //         //将原本M矩阵中的满足attributes条件的以及不都为0的列的条件的元素填到一个新的矩阵中，称为Mreduced，该矩阵事宜个长宽均为result.length的方阵
    //         mA(j, i) = lsssPolicyParameter->getLSSSMatrix()[result[j]][cols[i]];
    //     }
    // }
    mA = mA.inverse();
    int column = mA.cols();
    // double *_b = get_identity_vector(column);
    Eigen::RowVectorXd mb(column);
    mb(0, 0) = 1;
    for (int i=1; i<column; ++i) {
        mb(0, i) = 0;
    }
    Eigen::MatrixXd res = mb * mA;
    double *data = res.data();
    assert(column == res.cols());
    std::vector<double> solution(data, data + column);

    std::vector<element_t> minSatisfiedOmegaElements(solution.size());
    for (int i = 0; i < minSatisfiedOmegaElements.size(); i++) {
        element_init_Zr(minSatisfiedOmegaElements[i], pairing);
        element_set_si(minSatisfiedOmegaElements[i], solution[i]);
        // minSatisfiedOmegaElements[i] = pairing.getZr().newElement((int) solution[i]).getImmutable();
    }

    std::unordered_map<std::string, element_t> omegaElementsMap;
    for (int i = 0; i < rows.size(); i++) {
        element_init_same_as(omegaElementsMap[leafAttributes[i]+'_'+std::to_string((rows[i]+1)%2)], minSatisfiedOmegaElements[i]);
        element_set(omegaElementsMap.at(leafAttributes[i]+'_'+std::to_string((rows[i]+1)%2)), minSatisfiedOmegaElements[i]);
        // omegaElementsMap.at(attribute) = minSatisfiedOmegaElements[i].duplicate().getImmutable();
    }
    // for (std::string attribute : attributes) {
    //     if (omegaElementsMap.find(attribute) == omegaElementsMap.end()) {
    //         element_init_Zr(omegaElementsMap[attribute], pairing);
    //         element_set0(omegaElementsMap.at(attribute));
    //         // omegaElementsMap.at(attribute) = pairing.getZr().newZeroElement().getImmutable();
    //     }
    // }
    return omegaElementsMap;
}