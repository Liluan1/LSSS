#include "AccessTreeNode.h"

int AccessTreeNode::numberOfLeafNodes;
AccessTreeNode *AccessTreeNode::GenerateAccessTree(const std::vector<std::vector<int>> accessPolicy, const std::vector<std::string> rhos) {
    std::map<std::string, std::string> collisionMap;
    for (std::string rho : rhos) {
        if (collisionMap.find(rho) != collisionMap.end()) {
            throw std::invalid_argument("Invalid access policy, rhos containing identical std::string: " + rho);
        } else {
            collisionMap[rho] = rho;
        }
    }
    numberOfLeafNodes = 0;
    AccessTreeNode *rootAccessTreeNode = new AccessTreeNode(accessPolicy, 0, rhos);
    if (numberOfLeafNodes != rhos.size()) {
        throw std::invalid_argument("Invalid access policy, number of leaf nodes " + std::to_string(numberOfLeafNodes) + " does not match number of rhos " + std::to_string(rhos.size()));
    }
    return rootAccessTreeNode;
}

bool AccessTreeNode::isAccessControlSatisfied(const std::vector<std::string> attributes) {
    if (!this->isLeafNode) {
        int satisfiedChildNumber = 0;
        for (AccessTreeNode *childNode : this->childNodes) {
            if (childNode->isAccessControlSatisfied(attributes)) {
                satisfiedChildNumber++;
            }
        }
        return (satisfiedChildNumber >= t);
    } else {
        for (std::string eachAttribute : attributes) {
            if (this->attribute == eachAttribute) {
                return true;
            }
        }
        return false;
    }
}