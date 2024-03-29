#pragma once 
#include <unordered_map>
#include <algorithm>
#include <Eigen/Dense>
#include <pbc.h>
#include "LSSSPolicyParameter.h"

/**
 * Created by Weiran Liu on 2016/7/21.
 *
 * LSSSPolicyEngine class that implements AccessControlEngine.
 * Since the implementations of function secretSharing, reconstructOmegas are the same in LSSS realization,
 * I create this abstract engine to cover all the same codes.
 */
class LSSSPolicyEngine {
  public:
    /**
     * @param pairing JPBC pairing
     * @param secret Secret s
     * @param accessControlParameter rootAccessTreeNode, accessPolicy, lsssMatrix and rhosParameter
     * @return
     */
    virtual std::unordered_map<std::string, element_t> secretSharing(pairing_t pairing, element_t secret,   
      AccessControlParameter *accessControlParameter) = 0;

    virtual std::unordered_map<std::string, element_t> reconstructOmegas(pairing_t pairing, 
      std::vector<std::string> attributes, AccessControlParameter *accessControlParameter) = 0;

    virtual AccessControlParameter *generateAccessControl(const std::vector<std::vector<int>> &accessPolicy, 
      const std::vector<std::string> &rhos) = 0;
};