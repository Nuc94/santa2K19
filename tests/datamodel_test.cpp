#include "gtest/gtest.h"
#include "../include/santa2K19/data_model.h"

//here we are to test the datamodel

const std::string DEFAULT_FILEPATH = "/home/nuc/Programmazione/Kaggle/Santa's Workshop Tour 2019/family_data.csv";

TEST(DATAMODEL_TEST, IniTest) {    
    DataModel DM(DEFAULT_FILEPATH);
}

TEST(DATAMODEL_TEST, CorrectFamilyChoiceTest) {    
    DataModel DM(DEFAULT_FILEPATH);
    ASSERT_EQ(DM.getFamilyChoice(0,0), 52);
    ASSERT_EQ(DM.getFamilyChoice(0,9), 28);
    ASSERT_EQ(DM.getFamilyChoice(4999,0), 13);
    ASSERT_EQ(DM.getFamilyChoice(4999,9), 47);
}

TEST(DATAMODEL_TEST, CorrectFamilyComponentsTest) {    
    DataModel DM(DEFAULT_FILEPATH);
    ASSERT_EQ(DM.getFamilyComponents(0), 4);
    ASSERT_EQ(DM.getFamilyComponents(1), 4);
    ASSERT_EQ(DM.getFamilyComponents(4998), 5);
    ASSERT_EQ(DM.getFamilyComponents(4999), 4);
}