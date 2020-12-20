#include "gtest/gtest.h"
#include "../include/santa2K19/data_model.h"
#include "../include/santa2K19/population.h"

const std::string DEFAULT_FILEPATH = "/home/nuc/Programmazione/Kaggle/Santa's Workshop Tour 2019/family_data.csv";

TEST( POPTEST, IniTest ) {
    DataModel DM(DEFAULT_FILEPATH);
    Population Pop(500,DM);
}

TEST( POPTEST, SizeTest ) {
    DataModel DM(DEFAULT_FILEPATH);
    Population Pop(500,DM);
    ASSERT_EQ(Pop.size(), 500);
}

TEST( POPTEST, SorTest ) {
    DataModel DM(DEFAULT_FILEPATH);
    Population Pop(500,DM);
    for (int i = 0; i < Pop.size() - 1; ++i) {
        ASSERT_TRUE( Pop.getFitnessAt(i) <= Pop.getFitnessAt(i+1) );
    }
}