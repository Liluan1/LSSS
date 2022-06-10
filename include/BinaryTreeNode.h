#pragma once 
#include <string>
#include <list>
#include <iostream>
#include <vector>

/**
 * Created by Weiran Liu on 2016/7/20.
 *
 * Binary tree node
 */
class BinaryTreeNode {
  public:
    enum NodeType {
        AND, OR, LEAF,
    };

    std::vector<int> getVector1() { return vector1; }
    void setVector1(std::vector<int> vector) { this->vector1 = vector; }
    std::vector<int> getVector0() { return vector0; }
    void setVector0(std::vector<int> vector) { this->vector0 = vector; }
    BinaryTreeNode *getParent() {
        return this->parent;
    }
    void setParent(BinaryTreeNode *parent) {
        this->parent = parent;
    }
    std::string getValue() {
        return value;
    }
    void setValue(std::string value) {
        this->value = value;
    }
    NodeType getType() {
        return this->type;
    }
    void setType(NodeType type) {
        this->type = type;
    }
    BinaryTreeNode *getLeft() {
        return this->left;
    }
    void setLeft(BinaryTreeNode *left) {
        this->left = left;
    }
    BinaryTreeNode *getRight() {
        return this->right;
    }
    void setRight(BinaryTreeNode *right) {
        this->right = right;
    }

    static void preOrderTraversal(BinaryTreeNode *root);

    static void postOrderTraversal(BinaryTreeNode *root);

    static void inOrderTraversal(BinaryTreeNode *root);

    static void updateParentPointer(BinaryTreeNode *root);

    static BinaryTreeNode *ReconstructBinaryTreeNode(std::vector<std::vector<int>> accessPolicy, std::vector<std::string> rhos);

    friend inline std::ostream &operator<<(std::ostream &os, const BinaryTreeNode &node) {
        switch (node.type) {
            case NodeType::LEAF: os << "LEAF"; break;
            case NodeType::AND: os << "AND"; break;
            case NodeType::OR: os << "OR"; break;
        }
        return os;
    }

  private:
    NodeType type;
    std::string value;	// for leaf node
    BinaryTreeNode *parent;
    BinaryTreeNode *left;
    BinaryTreeNode *right;
    std::vector<int> vector1;
    std::vector<int> vector0;
};
