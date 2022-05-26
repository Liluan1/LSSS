#pragma once 
#include <string>
#include <vector>
#include "AccessTreeNode.h"

/**
 * Created by Weiran Liu on 2016/7/19.
 *
 * Generic access control parameters.
 */

class AccessControlParameter {
  private:
    //The Access Tree
    class AccessTreeNode *rootAccessTreeNode;
    //The access policy represented by int array
  protected:
    std::vector<std::vector<int>> accessPolicy;
    //Rho map
    std::vector<std::string> rhos;

    AccessControlParameter(class AccessTreeNode *accessTreeNode, std::vector<std::vector<int>> accessPolicy, 
      std::vector<std::string> rhos) {
        this->rootAccessTreeNode = accessTreeNode;
        this->accessPolicy = accessPolicy;
        //Copy rhos
        this->rhos = rhos;
    }

  public:
    std::vector<std::string> getRhos() {
        return this->rhos;
    }

    std::vector<std::vector<int>> getAccessPolicy() { return this->accessPolicy; }

    std::vector<std::string> minSatisfiedAttributeSet(std::vector<std::string> attributes);

    AccessTreeNode *getRootAccessTreeNode() {
        return this->rootAccessTreeNode;
    }
};