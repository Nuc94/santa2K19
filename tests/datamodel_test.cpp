#include "gtest/gtest.h"
#include "../include/santa2K19/data_model.h"

//here we are to test the datamodel

const std::string DEFAULT_FILEPATH = "/home/nuc/Programmazione/Kaggle/Santa's Workshop Tour 2019/family_data.csv";

TEST(DATAMODEL_TEST, IniTest) {    
    DataModel DM(DEFAULT_FILEPATH);
}

TEST(DATAMODEL_TEST, CorrectFamilyChoiceTest) {    
    DataModel DM(DEFAULT_FILEPATH);
    ASSERT_EQ(DM.getFamilyChoice(0,0), 52 - 1);
    ASSERT_EQ(DM.getFamilyChoice(0,9), 28 - 1);
    ASSERT_EQ(DM.getFamilyChoice(4999,0), 13 - 1);
    ASSERT_EQ(DM.getFamilyChoice(4999,9), 47 - 1);
}

TEST(DATAMODEL_TEST, CorrectFamilyComponentsTest) {    
    DataModel DM(DEFAULT_FILEPATH);
    ASSERT_EQ(DM.getFamilyComponents(0), 4);
    ASSERT_EQ(DM.getFamilyComponents(1), 4);
    ASSERT_EQ(DM.getFamilyComponents(4998), 5);
    ASSERT_EQ(DM.getFamilyComponents(4999), 4);
}

TEST(DATAMODEL_TEST, CorrectFamilyDayChoiceLevelTest) {    
    DataModel DM(DEFAULT_FILEPATH);
    ASSERT_EQ(DM.getFamilyDayChoiceLevel(0,27), 9);
    ASSERT_EQ(DM.getFamilyDayChoiceLevel(0,51), 0);
    ASSERT_EQ(DM.getFamilyDayChoiceLevel(0,11), 2);
    ASSERT_EQ(DM.getFamilyDayChoiceLevel(0,74), 5);
    ASSERT_EQ(DM.getFamilyDayChoiceLevel(0,18), 10);
    ASSERT_EQ(DM.getFamilyDayChoiceLevel(0,46), 10);
    ASSERT_EQ(DM.getFamilyDayChoiceLevel(0,47), 10);
    ASSERT_EQ(DM.getFamilyDayChoiceLevel(0,99), 10);

    ASSERT_EQ(DM.getFamilyDayChoiceLevel(4999,46), 9);
    ASSERT_EQ(DM.getFamilyDayChoiceLevel(4999,12), 0);
    ASSERT_EQ(DM.getFamilyDayChoiceLevel(4999,24), 2);
    ASSERT_EQ(DM.getFamilyDayChoiceLevel(4999,39), 5);
    ASSERT_EQ(DM.getFamilyDayChoiceLevel(4999,18), 10);
    ASSERT_EQ(DM.getFamilyDayChoiceLevel(4999,45), 10);
    ASSERT_EQ(DM.getFamilyDayChoiceLevel(4999,47), 10);
    ASSERT_EQ(DM.getFamilyDayChoiceLevel(4999,99), 10);

    ASSERT_EQ(DM.getFamilyDayChoiceLevel(4977,99), 7);
    
}