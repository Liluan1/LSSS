#include "AccessControlTest.h"
#include "AccessControlException.h"

const char *TEST_PAIRING_PARAMETERS_PATH_a_80_256 = "../params/a_80_256.properties";
const char *TEST_PAIRING_PARAMETERS_PATH_a1_2_128 = "../params/a1_2_128.properties";
const char *TEST_PAIRING_PARAMETERS_PATH_a1_3_128 = "../params/a1_3_128.properties";
void AccessControlTest::runAllTests(pbc_param_t param) {
  pairing_t pairing;
  pairing_init_pbc_param(pairing, param);
  
  //test satisfied access control
  try_valid_access_policy(pairing, 1, example_1, example_1_rho, example_1_satisfied_1);
  try_valid_access_policy(pairing, 2, example_1, example_1_rho, example_1_satisfied_2);
  try_valid_access_policy(pairing, 3, example_2, example_2_rho, example_2_satisfied_1);
  try_valid_access_policy(pairing, 4, example_2, example_2_rho, example_2_satisfied_2);
  try_valid_access_policy(pairing, 5, example_3, example_3_rho, example_3_satisfied_1);
  try_valid_access_policy(pairing, 6, example_3, example_3_rho, example_3_satisfied_2);
  try_valid_access_policy(pairing, 7, example_4, example_4_rho, example_4_satisfied_1);
  try_valid_access_policy(pairing, 8, example_4, example_4_rho, example_4_satisfied_2);
  try_valid_access_policy(pairing, 9, example_5, example_5_rho, example_5_satisfied_1);

  //test unsatisfied access control
  try_invalid_access_policy(pairing, 1, example_1, example_1_rho, example_1_unsatisfied_1);
  try_invalid_access_policy(pairing, 2, example_2, example_2_rho, example_2_unsatisfied_1);
  try_invalid_access_policy(pairing, 3, example_2, example_2_rho, example_2_unsatisfied_2);
  try_invalid_access_policy(pairing, 4, example_2, example_2_rho, example_2_unsatisfied_3);
  try_invalid_access_policy(pairing, 5, example_3, example_3_rho, example_3_unsatisfied_1);
  try_invalid_access_policy(pairing, 6, example_4, example_4_rho, example_4_unsatisfied_1);
  try_invalid_access_policy(pairing, 7, example_4, example_4_rho, example_4_unsatisfied_2);
  try_invalid_access_policy(pairing, 8, example_5, example_5_rho, example_5_unsatisfied_1);
  try_invalid_access_policy(pairing, 9, example_5, example_5_rho, example_5_unsatisfied_2);
}

void AccessControlTest::try_valid_access_policy(pairing_t pairing, int testIndex, const std::vector<std::vector<int>> accessPolicy, const std::vector<std::string> rhos, const std::vector<std::string> attributeSet) {
  try {
    //Access Policy Generation
    AccessControlParameter *accessControlParameter = accessControlEngine->generateAccessControl(accessPolicy, rhos);
    //SecretSharing
    element_t secret;
    element_init_Zr(secret, pairing);
    element_random(secret);
    // std::cout << "Generated Secret s = " << element_printf("%d", secret) << std::endl;
    std::unordered_map<std::string, element_t> lambdaElementsMap = accessControlEngine->secretSharing(pairing, secret, accessControlParameter);

    //Secret Reconstruction
    std::unordered_map<std::string, element_t> omegaElementsMap = accessControlEngine->reconstructOmegas(pairing, attributeSet, accessControlParameter);
    element_t reconstructedSecret;
    element_init_Zr(reconstructedSecret, pairing);
    element_set0(reconstructedSecret);
    for (std::string eachAttribute : attributeSet) {
      if (omegaElementsMap.find(eachAttribute) != omegaElementsMap.end()) {
        element_t e;
        element_init_same_as(e, reconstructedSecret);
        element_mul_zn(e, lambdaElementsMap.at(eachAttribute), omegaElementsMap.at(eachAttribute));
        element_add(reconstructedSecret, reconstructedSecret, e);
      }
    }
    // std::cout << "Reconstruct Secret s = " << element_printf("%d", reconstructedSecret) << std::endl;
    if (element_cmp(reconstructedSecret, secret) != 0) {
      std::cout << "Access Policy with Combined Gate Satisfied Test " << testIndex << ", Reconstructed Secret Wrong..." << std::endl;
      exit(0);
    }
    std::cout << "Access Policy with Combined Gate Satisfied Test " << testIndex << " Passed." << std::endl;
  } catch (std::exception& e) {
    std::cout << "Access Policy with Combined Gate Satisfied Test " << testIndex << ", Error for getting Exceptions: " << e.what() << std::endl;
    exit(0);
  }
}

void AccessControlTest::try_invalid_access_policy(pairing_t pairing, int testIndex, const std::vector<std::vector<int>> accessPolicy, const std::vector<std::string> rhos, const std::vector<std::string> attributeSet) {
  try {
    //Access Policy Generation
    AccessControlParameter *accessControlParameter = accessControlEngine->generateAccessControl(accessPolicy, rhos);
    //SecretSharing
    element_t secret;
    element_init_Zr(secret, pairing);
    element_random(secret);
    // out.println("Generated Secret s = " + secret);
    std::unordered_map<std::string, element_t> lambdaElementsMap = accessControlEngine->secretSharing(pairing, secret, accessControlParameter);
    // Secret Reconstruction
    std::unordered_map<std::string, element_t> omegaElementsMap = accessControlEngine->reconstructOmegas(pairing, attributeSet, accessControlParameter);
    element_t reconstructedSecret;
    element_init_Zr(reconstructedSecret, pairing);
    element_set0(reconstructedSecret);
    for (std::string eachAttribute : attributeSet) {
      if (omegaElementsMap.find(eachAttribute) != omegaElementsMap.end()) {
        element_t e;
        element_mul_zn(e, lambdaElementsMap.at(eachAttribute), omegaElementsMap.at(eachAttribute));
        element_add(reconstructedSecret, reconstructedSecret, e);
      }
    }
    std::cout << "Access Policy with Combined Gate Unsatisfied Test " << testIndex << ", Error for not getting Exceptions: " << std::endl;
    exit(0);
  } catch (AccessControlException &e) {
    std::cout << "Access Policy with Combined Gate Unsatisfied Test " << testIndex << " Passed." << std::endl;
  }
}

void AccessControlTest::testLSSSLW10Engine() {
  this->accessControlEngine = new LSSSLW10Engine();
  char param[1024];
  pbc_param_t pbc_param;
  FILE* file = fopen(TEST_PAIRING_PARAMETERS_PATH_a_80_256, "r");
  assert(file != NULL);
  size_t count = fread(param, 1, 1024, file);
  fclose(file);
  assert(count != 0);
  pbc_param_init_set_buf(pbc_param, param, count);
  runAllTests(pbc_param);
}