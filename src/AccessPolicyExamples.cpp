#include "AccessPolicyExamples.h"

/**
 * Created by Weiran Liu on 2016/11/17.
 *
 * Access policy examples, used for testing AccessControlEngine and Attribute-Based Encryption schemes.
 */
const std::vector<std::vector<int>> example_1 = {
        {2, 2, 1, 2},
        {2, 2, -1, -2},
        {2, 1, -3, -4},        
};
const std::vector<std::string> example_1_rho = {
        "0", "1", "2", "3",
};
const std::vector<std::string> example_1_satisfied_1 = {"0", "1", "2"};
const std::vector<std::string> example_1_satisfied_2 = {"0", "1", "2", "3"};
const std::vector<std::string> example_1_unsatisfied_1 = {"1", "2", "3"};

const std::vector<std::vector<int>> example_2 = {
        {2, 2, 1, 2},
        {2, 2, 3, 4},
        {2, 2, 5, 6},
        {2, 2, 7, -1},
        {2, 1, 8, -2},
        {2, 2, -3, -4},
        {2, 1, 9, -5},
        {2, 2, -6, -7},
        {2, 1, -8, -9},
        {2, 1, 10, -10},
        {2, 1, -11, -12},
};
const std::vector<std::string> example_2_rho = {
        "2", "5", "6", "7", "11", "0", "1", "3", "4", "10", "8", "9",
};
const std::vector<std::string> example_2_satisfied_1 = {"0", "1", "2", "4", "6", "7", "10"};
const std::vector<std::string> example_2_satisfied_2 = {"0", "1", "2", "5", "4", "6", "7", "8", "9", "10", "11"};
const std::vector<std::string> example_2_unsatisfied_1 = {"0", "1", "2", "6", "7", "10"};
const std::vector<std::string> example_2_unsatisfied_2 = {"0", "1", "2", "4", "6", "10"};
const std::vector<std::string> example_2_unsatisfied_3 = {"0", "1", "2", "3", "6", "7"};

const std::vector<std::vector<int>> example_3 = {
        {2, 2, 1, 2},
        {2, 2, -1, -2},
        {2, 1, -3, -4}
};
const std::vector<std::string> example_3_rho = {
        "school:pku", "professor", "academy:software", "academy:computer",
};
const std::vector<std::string> example_3_satisfied_1 = {
        "school:pku", "professor", "academy:software",
};
const std::vector<std::string> example_3_satisfied_2 = {
        "school:pku", "professor", "academy:software", "academy:computer",
};
const std::vector<std::string> example_3_unsatisfied_1 = {
        "professor", "academy:software", "academy:computer",
};

const std::vector<std::vector<int>> example_4 = {
        {2, 1, 1, 2},
        {2, 2, -1, -2},
        {2, 2, -3, -4},
};
const std::vector<std::string> example_4_rho = {
        "school:pku", "academy:software", "school:mit", "academy:computer",
};
const std::vector<std::string> example_4_satisfied_1 = {
        "school:pku", "academy:software",
};
const std::vector<std::string> example_4_satisfied_2 = {
        "school:pku", "school:mit", "academy:computer",
};
const std::vector<std::string> example_4_unsatisfied_1 = {
        "school:pku",
};
const std::vector<std::string> example_4_unsatisfied_2 = {
        "school:pku", "academy:computer",
};

const std::vector<std::vector<int>> example_5 = {
        {2, 2, 1, -1},          {2, 2, 2, -2},          {2, 2, 3, -3},          {2, 2, 4, -4},
        {2, 2, 5, -5},          {2, 2, 6, -6},          {2, 2, 7, -7},          {2, 2, 8, -8},
        {2, 2, 9, -9},          {2, 2, 10, -10},        {2, 2, 11, -11},        {2, 2, 12, -12},
        {2, 2, 13, -13},        {2, 2, 14, -14},        {2, 2, 15, -15},        {2, 2, 16, -16},
        {2, 2, 17, -17},        {2, 2, 18, -18},        {2, 2, 19, -19},        {2, 2, 20, -20},
        {2, 2, 21, -21},        {2, 2, 22, -22},        {2, 2, 23, -23},        {2, 2, 24, -24},
        {2, 2, 25, -25},        {2, 2, 26, -26},        {2, 2, 27, -27},        {2, 2, 28, -28},
        {2, 2, 29, -29},        {2, 2, 30, -30},        {2, 2, 31, -31},        {2, 2, 32, -32},
        {2, 2, 33, -33},        {2, 2, 34, -34},        {2, 2, 35, -35},        {2, 2, 36, -36},
        {2, 2, 37, -37},        {2, 2, 38, -38},        {2, 2, 39, -39},        {2, 2, 40, -40},
        {2, 2, 41, -41},        {2, 2, 42, -42},        {2, 2, 43, -43},        {2, 2, 44, -44},
        {2, 2, 45, -45},        {2, 2, 46, -46},        {2, 2, 47, -47},        {2, 2, 48, -48},
        {2, 2, -49, -50},
};
const std::vector<std::string> example_5_rho = {
        "A_00", "A_01", "A_02", "A_03", "A_04", "A_05", "A_06", "A_07", "A_08", "A_09", 
        "A_10", "A_11", "A_12", "A_13", "A_14", "A_15", "A_16", "A_17", "A_18", "A_19",
        "A_20", "A_21", "A_22", "A_23", "A_24", "A_25", "A_26", "A_27", "A_28", "A_29",
        "A_30", "A_31", "A_32", "A_33", "A_34", "A_35", "A_36", "A_37", "A_38", "A_39",
        "A_40", "A_41", "A_42", "A_43", "A_44", "A_45", "A_46", "A_47", "A_48", "A_49",
};
const std::vector<std::string> example_5_satisfied_1 = {
        "A_00", "A_01", "A_02", "A_03", "A_04", "A_05", "A_06", "A_07", "A_08", "A_09",
        "A_10", "A_11", "A_12", "A_13", "A_14", "A_15", "A_16", "A_17", "A_18", "A_19",
        "A_20", "A_21", "A_22", "A_23", "A_24", "A_25", "A_26", "A_27", "A_28", "A_29",
        "A_30", "A_31", "A_32", "A_33", "A_34", "A_35", "A_36", "A_37", "A_38", "A_39",
        "A_40", "A_41", "A_42", "A_43", "A_44", "A_45", "A_46", "A_47", "A_48", "A_49",
};
const std::vector<std::string> example_5_unsatisfied_1 = {
        "A_00", "A_01", "A_02", "A_03", "A_04", "A_05", "A_06", "A_07", "A_08", "A_09",
        "A_10", "A_11", "A_12", "A_13", "A_14", "A_15", "A_16", "A_17", "A_18", "A_19",
        "A_20", "A_21", "A_22", "A_23", "A_24", "A_25", "A_26", "A_27", "A_28", "A_29",
        "A_30", "A_31", "A_32", "A_33", "A_34", "A_35", "A_36", "A_37", "A_38", "A_39",
        "A_40", "A_41", "A_42", "A_43", "A_44", "A_45", "A_46", "A_47", "A_48",
};
const std::vector<std::string> example_5_unsatisfied_2 = {
                                        "A_04", "A_05", "A_06", "A_07", "A_08", "A_09",
        "A_10", "A_11", "A_12", "A_13", "A_14", "A_15", "A_16", "A_17", "A_18", "A_19",
        "A_20", "A_21", "A_22", "A_23", "A_24", "A_25", "A_26", "A_27", "A_28", "A_29",
        "A_30", "A_31", "A_32", "A_33", "A_34",                 "A_37", "A_38", "A_39",
        "A_40", "A_41", "A_42", "A_43", "A_44", "A_45", "A_46", "A_47", "A_48", "A_49",
};
