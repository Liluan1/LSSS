#pragma once 
#include <exception>
#include <iostream>
#include <pbc.h>
#include <fstream>
#include <cassert>
#include "LSSSLW10Engine.h"
#include "AccessPolicyExamples.h"

/**
 * Created by Weiran Liu on 2016/7/20.
 *
 * Access control engine test.
 */
class AccessControlTest {
 public: 
  void runAllTests(pbc_param_t param);

  void testLSSSLW10Engine();

 private: 
  void try_valid_access_policy(pairing_t pairing, int testIndex, const std::vector<std::vector<int>> accessPolicy, 
    const std::vector<std::string> rhos, const std::vector<std::string> attributeSet) ;

  void try_invalid_access_policy(pairing_t pairing, int testIndex, const std::vector<std::vector<int>> accessPolicy, const std::vector<std::string> rhos, const std::vector<std::string> attributeSet) ;

  LSSSLW10Engine *accessControlEngine;
};
