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
    Population Pop(499,DM);
    ASSERT_EQ(Pop.size(), 499);
}

TEST( POPTEST, BigPopTest ) {
    const int pop_size_test = 5000;
    DataModel DM(DEFAULT_FILEPATH);
    Population Pop(pop_size_test,DM);
    ASSERT_EQ(Pop.size(), pop_size_test);
}

TEST( POPTEST, SorTest ) {
    DataModel DM(DEFAULT_FILEPATH);
    Population Pop(500,DM);
    for (int i = 0; i < Pop.size() - 1; ++i) {
        ASSERT_TRUE( Pop.getFitnessAt(i) <= Pop.getFitnessAt(i+1) );
    }
}

TEST( POPTEST, TourSelBaseTest ) {
    DataModel DM(DEFAULT_FILEPATH);
    Population Pop(10,DM);
    int selection;
    selection = Pop.tournamentSelect<2>(-1);
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

TEST( OFFPOLICYTEST, IniTest ) {
    OffspringPolicy OF(&Population::fillByElitism);
    OffspringPolicy OFF(&Population::fillByRankSelect);
    OffspringPolicy OFFS(&Population::fillByTournamentSelect<6>);
}

TEST( OFFPOLICYTEST, RankSelTest ) {
    
    const int test_pop_size = 5000;
    std::vector<int> sel_target;

    OffspringPolicy OF(&Population::fillByRankSelect);

    DataModel DM(DEFAULT_FILEPATH);

    Population Pop(test_pop_size,DM);

    sel_target.reserve( test_pop_size );

    OF.applySelection(&Pop, sel_target, test_pop_size);

    ASSERT_EQ( sel_target.size(), test_pop_size );

    for(int elem : sel_target) ASSERT_LE(elem, test_pop_size);

    //Then I would expect the aveage value from my new selection to
    //be greater or equal than half the test size

    int avg_sel = 0;
    for(int elem : sel_target) avg_sel += elem;
    avg_sel = avg_sel / sel_target.size();

    EXPECT_LE(avg_sel, test_pop_size / 2);
}

TEST( OFFPOLICYTEST, BinTourSelTest ) {
    
    const int test_pop_size = 5000;
    std::vector<int> sel_target;

    OffspringPolicy OF(&Population::fillByTournamentSelect<2>);

    DataModel DM(DEFAULT_FILEPATH);

    Population Pop(test_pop_size,DM);

    sel_target.reserve( test_pop_size );

    OF.applySelection(&Pop, sel_target, test_pop_size);

    ASSERT_EQ( sel_target.size(), test_pop_size );

    for(int elem : sel_target) ASSERT_LE(elem, test_pop_size);

    //Then I would expect the aveage value from my new selection to
    //be greater or equal than half the test size

    int avg_sel = 0;
    for(int elem : sel_target) avg_sel += elem;
    avg_sel = avg_sel / sel_target.size();

    EXPECT_LE(avg_sel, test_pop_size / 2);
}

TEST( OFFPOLICYTEST, EliTest ) {
    
    const int test_pop_size = 5000;
    std::vector<int> sel_target;

    OffspringPolicy OF(&Population::fillByElitism);

    DataModel DM(DEFAULT_FILEPATH);
    Population Pop(test_pop_size,DM);

    sel_target.reserve( test_pop_size );
    OF.applySelection(&Pop, sel_target, test_pop_size);

    ASSERT_EQ( sel_target.size(), test_pop_size );

    for(int elem : sel_target) ASSERT_LE(elem, test_pop_size);
}

TEST( OFFPOLICYTEST, EliTestOverloop ) {
    
    const int test_pop_size = 50;
    const int sel_target_size = 5000;
    std::vector<int> sel_target;

    OffspringPolicy OF(&Population::fillByElitism);

    DataModel DM(DEFAULT_FILEPATH);
    Population Pop(test_pop_size,DM);

    sel_target.reserve( sel_target_size );
    OF.applySelection(&Pop, sel_target, sel_target_size);

    ASSERT_EQ( sel_target.size(), sel_target_size );

    for(int elem : sel_target) ASSERT_LE(elem, test_pop_size);
}

TEST( EVOTEST, NextPopIniTest ) {
    const int test_pop_size = 50;
    const int next_pop_size = 70;

    evolution_policies ev_pol;

    ev_pol.push_back( std::make_pair(next_pop_size, std::make_unique<OffspringPolicy>( &Population::fillByTournamentSelect<2>, &Chromosome::randomSinglePointCrossover, &Chromosome::randomElementaryMutation ) ) );

    DataModel DM(DEFAULT_FILEPATH);
    Population Pop(test_pop_size,DM);

    Pop.evolve(ev_pol);

    ASSERT_EQ(next_pop_size, Pop.size());

}