#pragma once
#include "LSSSPolicyEngine.h"

class LSSSDKW21: public LSSSPolicyEngine {
    AccessControlParameter *generateAccessControl(const std::vector<std::vector<int>> &accessPolicy, 
      const std::vector<std::string> &rhos);

    std::unordered_map<std::string, element_t> secretSharing(pairing_t pairing, element_t secret, 
      AccessControlParameter *accessControlParameter);

    std::unordered_map<std::string, element_t> reconstructOmegas(pairing_t pairing, std::vector<std::string> attributes, 
      AccessControlParameter *accessControlParameter);
};