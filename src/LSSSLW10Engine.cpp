#include "LSSSLW10Engine.h"

AccessControlParameter *LSSSLW10Engine::generateAccessControl(const std::vector<std::vector<int>> &accessPolicy, 
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
    int c = 1;
    std::vector<int> vector{1};
    // We begin by labeling the root node of the tree with the vector (1) (a vector of length 1).
    rootBinaryTreeNode->setVector1(vector);

    std::queue<BinaryTreeNode*> queue;
    queue.push(rootBinaryTreeNode);

    while (!queue.empty()) {
        BinaryTreeNode *p = queue.front();
        queue.pop();
        if (p->getType() == BinaryTreeNode::NodeType::AND) {
            // If the parent node is AND gate labeled by the vector v
            int size = p->getVector1().size();
            std::vector<int> pv;
            // We pad v with 0's at the end (if necessary) to make it of length c.
            if (size < c) {
                pv = p->getVector1();
                for (int i = 0; i < c - size; i++) {
                    pv.push_back(0);
                }
            } else {
                pv = p->getVector1();
            }
            // Then we label one of its children (right children) with the vector v|1
            BinaryTreeNode *right = p->getRight();
            std::vector<int> rv(pv);
            // rv.assign(pv);
            rv.push_back(1);
            right->setVector1(rv);
            queue.push(right);

            // Then we label one of its children (left children) with the vector (0,...,0)|-1
            BinaryTreeNode *left = p->getLeft();
            std::vector<int> lv;
            for (int i = 0; i < c; i++) {
                lv.push_back(0);
            }
            lv.push_back(-1);
            left->setVector1(lv);
            queue.push(left);
            // We now increment the value of c by 1.
            c += 1;
        } else if (p->getType() == BinaryTreeNode::NodeType::OR) {
            //If the parent node is an OR gate labeled by the vector v
            BinaryTreeNode *left = p->getLeft();
            std::vector<int> lv(p->getVector1());
            //then we also label its (left) children by v (and the value of c stays the same)
            // lv.assign(p->getVector1());
            left->setVector1(lv);
            queue.push(left);

            BinaryTreeNode *right = p->getRight();
            std::vector<int> rv(p->getVector1());
            //then we also label its (right) children by v (and the value of c stays the same)
            // rv.assign(p->getVector1());
            right->setVector1(rv);
            queue.push(right);
        } else {
            // leaf node
            rows += 1;
            int size = p->getVector1().size();
            maxLen = size > maxLen ? size : maxLen;
            if (map.find(p->getValue()) != map.end()) {
                map.at(p->getValue()).push_back(p->getVector1());
            } else {
                std::list<std::vector<int>> list;
                list.push_back(p->getVector1());
                map[p->getValue()] = list;
            }
        }
    }

    for (auto &pair : map) {
        std::list<std::vector<int>> &v = pair.second;
        for (std::vector<int> &aV : v) {
            int size = aV.size();
            if (size < maxLen) {
                for (int j = 0; j < maxLen - size; j++) {
                    aV.push_back(0);
                }
            }
        }
    }

    // Construct the lsss Matrix
    std::vector<std::vector<int>> lsssMatrix(rows);
    std::vector<std::string> rhosParameter(rhos.size());
    int i = 0;
    for (auto &pair : map) {
        std::list<std::vector<int>> v = pair.second;
        for (std::vector<int> aV : v) {
            rhosParameter[i] = pair.first;
            lsssMatrix[i] = aV;
            // for (int k = 0; k < maxLen; k++) {
            //     lsssMatrix[i][k] = aV.front();
            //     aV.pop_front();
            // }
            i += 1;
        }
    }
    return new LSSSPolicyParameter(rootAccessTreeNode, accessPolicy, lsssMatrix, rhosParameter);
}

std::unordered_map<std::string, element_t> LSSSLW10Engine::secretSharing(pairing_t pairing, element_t secret, 
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
        element_init_same_as(lambdaElementsMap[rhos.at(i/2) + std::to_string((i+1)%2)], elementsLambda);
        element_set(lambdaElementsMap[rhos.at(i/2) + std::to_string((i+1)%2)], elementsLambda);
    }
    return lambdaElementsMap;
}

std::unordered_map<std::string, element_t> LSSSLW10Engine::reconstructOmegas(pairing_t pairing, 
  std::vector<std::string> attributes, AccessControlParameter *accessControlParameter) {
    LSSSPolicyParameter *lsssPolicyParameter = static_cast<LSSSPolicyParameter*>(accessControlParameter);
    
    std::vector<std::string> minSatisfiedAttributes = lsssPolicyParameter->minSatisfiedAttributeSet(attributes);
    std::vector<std::string> leafAttributes = lsssPolicyParameter->getRhos();
    std::vector<int> rows(minSatisfiedAttributes.size());
    int counter = 0;
    for (int i = 0; i < leafAttributes.size(); i++){
        for (std::string minSatisfiedAttribute : minSatisfiedAttributes) {
            if (leafAttributes[i] == minSatisfiedAttribute) {
                //比较L矩阵和获得的S参数中各个元素，记下所有相同的元素对应的在数组中的位置，并生成一个新的矩阵，把相同的元素存在一个叫做result的数组之中，长度为counter
                rows[counter++] = i;
            }
        }
    }

    std::vector<int> result(rows);
    //filter M to rows from all zero cols and transpose it
    //eliminate all zero cols
    counter = 0;
    std::vector<int> cols(result.size());
    for(int j = 0; j < lsssPolicyParameter->getColumn(); j++){
        for (int aResult : result) {
            if (lsssPolicyParameter->getLSSSMatrix()[aResult][j] != 0) {
                if (counter == cols.size()) {
                    //此时矩阵不满足解密的条件
                    throw std::invalid_argument("Invalid access structure or attributes. Unable to reconstruct coefficients.");
                }
                //把不都为0的列数调出来，把列数j存到叫做的cols的数组之中,此时counter的含义是代表了新生成的M矩阵的列数
                cols[counter++] = j;
                break;
            }
        }
    }
    assert(counter == result.size());
    // double **Mreduced = new double*[counter];
    // for (int i = 0; i < counter; ++i) {
    //     Mreduced[i] = new double[counter];
    // }
    //solve the linear system
    Eigen::MatrixXd mA(counter, counter);
    for(int i = 0; i < result.size(); i++){
        for(int j = 0; j < result.size(); j++){
            //将原本M矩阵中的满足attributes条件的以及不都为0的列的条件的元素填到一个新的矩阵中，称为Mreduced，该矩阵事宜个长宽均为result.length的方阵
            mA(j, i) = lsssPolicyParameter->getLSSSMatrix()[result[j]][cols[i]];
        }
    }
    mA = mA.inverse();
    int column = mA.cols();
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
        for (std::string attribute : attributes) {
            if (leafAttributes[rows[i]] == attribute) {
                element_init_same_as(omegaElementsMap[attribute], minSatisfiedOmegaElements[i]);
                element_set(omegaElementsMap.at(attribute), minSatisfiedOmegaElements[i]);
                // omegaElementsMap.at(attribute) = minSatisfiedOmegaElements[i].duplicate().getImmutable();
            }
        }
    }
    for (std::string attribute : attributes) {
        if (omegaElementsMap.find(attribute) == omegaElementsMap.end()) {
            element_init_Zr(omegaElementsMap[attribute], pairing);
            element_set0(omegaElementsMap.at(attribute));
            // omegaElementsMap.at(attribute) = pairing.getZr().newZeroElement().getImmutable();
        }
    }
    return omegaElementsMap;
}