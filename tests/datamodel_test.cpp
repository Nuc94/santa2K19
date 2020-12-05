#include "gtest/gtest.h"
#include "../include/santa2K19/data_model.h"

//here we are to test the datamodel

const std::string DEFAULT_FILEPATH = "/home/nuc/Programmazione/Kaggle/Santa's Workshop Tour 2019/family_data.csv";

TEST(DATAMODEL_TEST, IniTest) {    
    DataModel DM(DEFAULT_FILEPATH);
}