#include "LSSSPolicyEngine.h"

std::unordered_map<std::string, element_t> LSSSPolicyEngine::secretSharing(pairing_t pairing, element_t secret, AccessControlParameter *accessControlParameter) {
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
    // elementsV[0] = secret.duplicate().getImmutable();
    for (int i = 1; i < elementsV.size(); i++) {
        element_init_Zr(elementsV[i], pairing);
        element_random(elementsV[i]);
        // elementsV[i] = pairing.getZr().newRandomElement().getImmutable();
    }
    // Secret share by matrix multiplication
    std::unordered_map<std::string, element_t> lambdaElementsMap;
    std::vector<std::string> rhos = lsssPolicyParameter->getRhos();
    for (int i=0; i<row; i++) {
        // element_t elementsLambda = pairing.getZr().newZeroElement().getImmutable();
        element_t elementsLambda;
        element_init_Zr(elementsLambda, pairing);
        element_set0(elementsLambda);
        for (int j=0; j<column; j++) {
            element_t e;
            element_init_same_as(e, elementsV[j]);
            element_mul_zn(e, elementLSSSMatrix[i][j], elementsV[j]);
            element_add(elementsLambda, elementsLambda, e);
            // elementsLambda = elementsLambda.add(elementLSSSMatrix[i][j].mulZn(elementsV[j])).getImmutable();
        }
        element_init_same_as(lambdaElementsMap[rhos[i]], elementsLambda);
        element_set(lambdaElementsMap[rhos[i]], elementsLambda);
        // lambdaElementsMap.at(lsssPolicyParameter->getRhos()[i]) =  elementsLambda;
    }
    return lambdaElementsMap;
}

std::unordered_map<std::string, element_t> LSSSPolicyEngine::reconstructOmegas(pairing_t pairing, std::vector<std::string> attributes, AccessControlParameter *accessControlParameter) {
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
                    throw "Invalid access structure or attributes. Unable to reconstruct coefficients.";
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
    double *_b = get_identity_vector(column);
    Eigen::RowVectorXd mb(column);
    for (int i=0; i<column; ++i) {
        mb(0, i) = _b[i];
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

double *LSSSPolicyEngine::get_identity_vector(int length) {
    // 该方法实现的功能是：生成矩阵求逆时等号右边的列向量，第一个数为1，剩下的都是0
    double *result = new double[length];
    result[0] = 1.0;
    for(int i = 1; i < length; i++) {
        result[i] = 0.0;
    }
    return result;
}