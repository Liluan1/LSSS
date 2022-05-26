#include "AccessControlParameter.h"
#include "AccessControlException.h"

std::vector<std::string> AccessControlParameter::minSatisfiedAttributeSet(std::vector<std::string> attributes) {
    if (!this->rootAccessTreeNode->isAccessControlSatisfied(attributes)) {
        throw AccessControlException("Give attribute set does not satisfy access policy");
    }
    bool *isRedundantAttribute = new bool[attributes.size()];
    int numOfMinAttributeSet = attributes.size();
    for (int i = 0; i < attributes.size(); i++) {
        isRedundantAttribute[i] = true;
        numOfMinAttributeSet--;
        std::vector<std::string> minAttributeSet(numOfMinAttributeSet);
        for (int j = 0, k = 0; j < attributes.size(); j++) {
            if (!isRedundantAttribute[j]) {
                minAttributeSet[k] = attributes[j];
                k++;
            }
        }
        if (!this->rootAccessTreeNode->isAccessControlSatisfied(minAttributeSet)) {
            numOfMinAttributeSet++;
            isRedundantAttribute[i] = false;
        }
    }
    std::vector<std::string> minAttributeSet(numOfMinAttributeSet);
    
    for (int j = 0, k = 0; j < attributes.size(); j++) {
        if (!isRedundantAttribute[j]) {
            minAttributeSet[k] = attributes[j];
            k++;
        }
    }
    return minAttributeSet;
}