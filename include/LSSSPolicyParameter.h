#pragma once 
#include <string>
#include <vector>
#include <iostream>
#include "AccessControlParameter.h"

/**
 * Created by Weiran Liu on 2016/7/18.
 *
 * LSSS policy parameter.
 */
class LSSSPolicyParameter: public AccessControlParameter {
  private: 
    //The LSSS matrix
    std::vector<std::vector<int>> lsssMatrix;
    //number of rows
    int row;
    //number of columns
    int column;

  public:
    LSSSPolicyParameter(AccessTreeNode *rootAccessTreeNode, std::vector<std::vector<int>> accessPolicy, std::vector<std::vector<int>> lsssMatrix, 
      std::vector<std::string> rhos) : AccessControlParameter(rootAccessTreeNode, accessPolicy, rhos) {
        this->row = lsssMatrix.size();
        this->column = lsssMatrix[0].size();
        this->lsssMatrix = lsssMatrix;
    }

    int getRow() {
        return this->row;
    }

    int getColumn() { return this->column; }

    std::vector<std::vector<int>> getLSSSMatrix(){
        return this->lsssMatrix;
    }

    std::vector<std::string> getRhos() {
        return this->rhos;
    }

    friend inline std::ostream &operator<<(std::ostream &os, const LSSSPolicyParameter &param) {
        for(int i=0; i<param.row; i++){
            os << i << " |";
            for(int j=0; j<param.column; j++){
                os << param.lsssMatrix[i][j] << ",";
            }
            os << "|, Rho[" << i << "] = " << param.rhos[i];
            os << std::endl;
        }
        return os;
    }
};
