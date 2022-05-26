#pragma once 
#include <string>
#include <map>
#include <vector>

/**
 * Created by Weiran Liu on 2016/7/19.
 *
 * Generic access tree node
 */

class AccessTreeNode {
  public: 
    static AccessTreeNode *GenerateAccessTree(const std::vector<std::vector<int>> accessPolicy, const std::vector<std::string> rhos);

    bool isAccessControlSatisfied(const std::vector<std::string> attributes);

    int getT() {
        return this->t;
    }

    int getN() {
        return this->childNodes.size();
    }

    AccessTreeNode *getChildNodeAt(int index) {
        return this->childNodes[index];
    }

    bool IsLeafNode() {
        return this->isLeafNode;
    }

    std::string getAttribute() {
        return this->attribute;
    }

    int getLabel() {
        return this->label;
    }

  private: 
    static int numberOfLeafNodes;
    std::vector<AccessTreeNode*> childNodes;
    int t;
    int label;
    std::string attribute;
    bool isLeafNode;

    AccessTreeNode(const int i, const std::string rho) {
        // this->childNodes = NULL;
        this->t = 0;
        this->label = i;
        this->isLeafNode = true;
        this->attribute = rho;
    }

    AccessTreeNode(const std::vector<std::vector<int>> accessPolicy, const int i, const std::vector<std::string> rhos) {
        const std::vector<int> accessPolicyNode = accessPolicy[i];
        if (accessPolicyNode[0] < accessPolicyNode[1]) {
            throw std::invalid_argument("Invalid access policy, n < t in the threahold gate " + i);
        }
        this->childNodes = std::vector<AccessTreeNode*>(accessPolicyNode[0]);
        this->t = accessPolicyNode[1];
        this->label = i;
        this->attribute = "";
        this->isLeafNode = false;
        int k = 0;
        // int length = sizeof(accessPolicyNode) / sizeof(accessPolicyNode[0]);
        for (int j = 2; j < accessPolicyNode.size(); j++) {
            if (accessPolicyNode[j] > 0) {
                this->childNodes[k] = new AccessTreeNode(accessPolicy, accessPolicyNode[j], rhos);
            } else if (accessPolicyNode[j] < 0) {
                numberOfLeafNodes++;
                this->childNodes[k] = new AccessTreeNode(accessPolicyNode[j], rhos[-accessPolicyNode[j] - 1]);
            } else {
                throw std::invalid_argument("Invalid access policy, containing access node with index 0");
            }
            k++;
        }
    }
};