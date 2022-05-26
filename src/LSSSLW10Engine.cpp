#include "LSSSLW10Engine.h"

AccessControlParameter *LSSSLW10Engine::generateAccessControl(std::vector<std::vector<int>> accessPolicy, std::vector<std::string> rhos) {
    // init access tree
    AccessTreeNode *rootAccessTreeNode = AccessTreeNode::GenerateAccessTree(accessPolicy, rhos);
    // reconstruct binary tree node
    BinaryTreeNode *rootBinaryTreeNode = BinaryTreeNode::ReconstructBinaryTreeNode(accessPolicy, rhos);

    // generate lsss matrix
    std::unordered_map<std::string, std::list<std::list<int>>> map;
    int maxLen = 0;
    int rows = 0;
    // We maintain a global counter variable c, which is initialized to 1.
    int c = 1;
    std::list<int> vector;
    // We begin by labeling the root node of the tree with the vector (1) (a vector of length 1).
    vector.push_back(1);
    rootBinaryTreeNode->setVector(vector);

    std::queue<BinaryTreeNode*> queue;
    queue.push(rootBinaryTreeNode);

    while (!queue.empty()) {
        BinaryTreeNode *p = queue.front();
        queue.pop();
        if (p->getType() == BinaryTreeNode::NodeType::AND) {
            // If the parent node is AND gate labeled by the vector v
            int size = p->getVector().size();
            std::list<int> pv;
            // We pad v with 0's at the end (if necessary) to make it of length c.
            if (size < c) {
                pv = p->getVector();
                for (int i = 0; i < c - size; i++) {
                    pv.push_back(0);
                }
            } else {
                pv = p->getVector();
            }
            // Then we label one of its children (right children) with the vector v|1
            BinaryTreeNode *right = p->getRight();
            std::list<int> rv(pv);
            // rv.assign(pv);
            rv.push_back(1);
            right->setVector(rv);
            queue.push(right);

            // Then we label one of its children (left children) with the vector (0,...,0)|-1
            BinaryTreeNode *left = p->getLeft();
            std::list<int> lv;
            for (int i = 0; i < c; i++) {
                lv.push_back(0);
            }
            lv.push_back(-1);
            left->setVector(lv);
            queue.push(left);
            // We now increment the value of c by 1.
            c += 1;
        } else if (p->getType() == BinaryTreeNode::NodeType::OR) {
            //If the parent node is an OR gate labeled by the vector v
            BinaryTreeNode *left = p->getLeft();
            std::list<int> lv(p->getVector());
            //then we also label its (left) children by v (and the value of c stays the same)
            // lv.assign(p->getVector());
            left->setVector(lv);
            queue.push(left);

            BinaryTreeNode *right = p->getRight();
            std::list<int> rv(p->getVector());
            //then we also label its (right) children by v (and the value of c stays the same)
            // rv.assign(p->getVector());
            right->setVector(rv);
            queue.push(right);
        } else {
            // leaf node
            rows += 1;
            int size = p->getVector().size();
            maxLen = size > maxLen ? size : maxLen;
            if (map.find(p->getValue()) != map.end()) {
                map.at(p->getValue()).push_back(p->getVector());
            } else {
                std::list<std::list<int>> list;
                list.push_back(p->getVector());
                map[p->getValue()] = list;
            }
        }
    }

    for (auto &pair : map) {
        std::list<std::list<int>> &v = pair.second;
        for (std::list<int> &aV : v) {
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
        std::list<std::list<int>> v = pair.second;
        for (std::list<int> aV : v) {
            rhosParameter[i] = pair.first;
            lsssMatrix[i].resize(maxLen);
            for (int k = 0; k < maxLen; k++) {
                lsssMatrix[i][k] = aV.front();
                aV.pop_front();
            }
            i += 1;
        }
    }
    return new LSSSPolicyParameter(rootAccessTreeNode, accessPolicy, lsssMatrix, rhosParameter);
}