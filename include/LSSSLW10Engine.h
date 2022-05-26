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
    AccessControlParameter *generateAccessControl(std::vector<std::vector<int>> accessPolicy, std::vector<std::string> rhos);
};
