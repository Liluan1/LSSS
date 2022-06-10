#pragma once 
#include <unordered_map>
#include <string>
#include <list>
#include <queue>
#include "LSSSPolicyEngine.h"
#include "AccessTreeNode.h"
#include "BinaryTreeNode.h"

/**
 * Created by Weiran Liu on 2016/7/20.
 *
 * This is the implementation of the linear secret sharing scheme proposed by Lewko and Waters in 2010.
 * Conference version: A. Lewko, B. Waters. Decentralizing attribute-based encryption. EUROCRYPT 2011, 568-588.
 * Full version: A. Lewko, B. Waters. Decentralizing attribute-based encryption. IACR Cryptology ePrint Achieve, 351, 2010.
 *
 * The original version of this code is written by the colleague of Cong Li,
 * which can be downloaded at https://github.com/cleverli2008/ABE.
 * I rewrite the code to meet my code architecture.
 */
class LSSSLW10Engine: public LSSSPolicyEngine {
  public:
    /**
     * @param accessPolicy Access policy matrix
     * @param rhos Leaf node value in string array
     * @return rootAccessTreeNode, accessPolicy, lsssMatrix and rhosParameter
     */
    AccessControlParameter *generateAccessControl(const std::vector<std::vector<int>> &accessPolicy, 
      const std::vector<std::string> &rhos);

    std::unordered_map<std::string, element_t> secretSharing(pairing_t pairing, element_t secret, 
      AccessControlParameter *accessControlParameter);

    std::unordered_map<std::string, element_t> reconstructOmegas(pairing_t pairing, std::vector<std::string> attributes, 
      AccessControlParameter *accessControlParameter);
};
