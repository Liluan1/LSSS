#include "BinaryTreeNode.h"
#include <cassert>

void BinaryTreeNode::preOrderTraversal(BinaryTreeNode *root){
    if(root == NULL){
        return;
    }
    std::cout << *root << std::endl;
    preOrderTraversal(root->getLeft());
    preOrderTraversal(root->getRight());
}

void BinaryTreeNode::postOrderTraversal(BinaryTreeNode *root){
    if(root == NULL){
        return;
    }
    postOrderTraversal(root->getLeft());
    std::cout << *root << std::endl;
    postOrderTraversal(root->getRight());
}

void BinaryTreeNode::inOrderTraversal(BinaryTreeNode *root){
    if(root == NULL){
        return;
    }
    inOrderTraversal(root->getLeft());
    inOrderTraversal(root->getRight());
    std::cout << *root << std::endl;
}

void BinaryTreeNode::updateParentPointer(BinaryTreeNode *root){
    BinaryTreeNode *p = root;
    BinaryTreeNode *left = root->getLeft();
    BinaryTreeNode *right = root->getRight();
    if(left != NULL){
        left->setParent(p);
        updateParentPointer(left);
    }
    if(right != NULL){
        right->setParent(p);
        updateParentPointer(right);
    }
}

BinaryTreeNode *BinaryTreeNode::ReconstructBinaryTreeNode(std::vector<std::vector<int>> accessPolicy, std::vector<std::string> rhos) {
    BinaryTreeNode **binaryNonLeafTreeNodes = new BinaryTreeNode*[accessPolicy.size()];
    for (int i = 0; i < accessPolicy.size(); i++) {
        binaryNonLeafTreeNodes[i] = new BinaryTreeNode();
        if (accessPolicy[i][0] == 2 && accessPolicy[i][1] == 2) {
            //AND GATE
            binaryNonLeafTreeNodes[i]->setType(NodeType::AND);
        } else if (accessPolicy[i][0] == 2 && accessPolicy[i][1] == 1) {
            //OR GATE
            binaryNonLeafTreeNodes[i]->setType(NodeType::OR);
        } else {
            assert(false);
        }
    }
    for (int i = 0; i < accessPolicy.size(); i++) {
        //link left node
        if (accessPolicy[i][2] > 0) {
            //link non-leaf nodes
            binaryNonLeafTreeNodes[i]->setLeft(binaryNonLeafTreeNodes[accessPolicy[i][2]]);
            binaryNonLeafTreeNodes[accessPolicy[i][2]]->setParent(binaryNonLeafTreeNodes[i]);
        } else {
            //link leaf node
            BinaryTreeNode *leafNode = new BinaryTreeNode();
            leafNode->setType(NodeType::LEAF);
            leafNode->setValue(rhos[-1 * accessPolicy[i][2] - 1]);
            binaryNonLeafTreeNodes[i]->setLeft(leafNode);
            leafNode->setParent(binaryNonLeafTreeNodes[i]);
        }
        //link right node
        if (accessPolicy[i][3] > 0) {
            //link non-leaf nodes
            binaryNonLeafTreeNodes[i]->setRight(binaryNonLeafTreeNodes[accessPolicy[i][3]]);
            binaryNonLeafTreeNodes[accessPolicy[i][3]]->setParent(binaryNonLeafTreeNodes[i]);
        } else {
            //link leaf node
            BinaryTreeNode *leafNode = new BinaryTreeNode();
            leafNode->setType(NodeType::LEAF);
            leafNode->setValue(rhos[-1 * accessPolicy[i][3] - 1]);
            binaryNonLeafTreeNodes[i]->setRight(leafNode);
            leafNode->setParent(binaryNonLeafTreeNodes[i]);
        }
    }
    return binaryNonLeafTreeNodes[0];
}