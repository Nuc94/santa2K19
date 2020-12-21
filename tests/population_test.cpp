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

TEST( POPTEST, RankSelSearchTest ) {
    DataModel DM(DEFAULT_FILEPATH);
    Population Pop(10,DM);
    int selection;
    const std::vector<int> & sel_weights = Pop.getRankSelWeigths();
    selection = searchLowLim(0, sel_weights.size(), 0, sel_weights);
    ASSERT_EQ(selection, 0);
    selection = searchLowLim(0, sel_weights.size(), 54, sel_weights);
    ASSERT_EQ(selection, 9);
}

TEST( POPTEST, RankSelAvoidTest10Size ) {
    DataModel DM(DEFAULT_FILEPATH);
    Population Pop(10,DM);
    int selection;
    int avoid = 0;
    const std::vector<int> & sel_weights = Pop.getRankSelWeigths();
    for(int i = 0; i < 10000; ++i) {
        selection = Pop.rankSelect(avoid);
        ASSERT_NE(selection, avoid);
    }
}