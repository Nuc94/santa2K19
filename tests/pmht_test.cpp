#include "gtest/gtest.h"
#include "../include/santa2K19/pmht.h"

//this test serves the purpose of seeing if I can instantiate a pmht
TEST( PMHT_TEST, IniTest ) {
    PMHT<10> init_pmht;
    ASSERT_EQ(init_pmht.size(), 0);
}

//This test serves the purpose of seeing if the insert function can be called
//without the system crashing
TEST(PMHT_TEST, InserTest) {
    PMHT<10> init_pmht;
    init_pmht.insert(4);
    init_pmht.insert(5);
    init_pmht.insert(6);
    init_pmht.insert(6);
    init_pmht.insert(-1);
    init_pmht.insert(0);
    init_pmht.insert(10);
}

TEST(PMHT_TEST, ContainTest) {
    PMHT<10> init_pmht;
    init_pmht.contains(-2);
    init_pmht.contains(3);
    init_pmht.contains(5);
    init_pmht.contains(9);
    init_pmht.contains(10);
    init_pmht.contains(11);
    init_pmht.contains(16);
}

TEST(PMHT_TEST, RemoveTest) {
    PMHT<10> init_pmht;
    init_pmht.remove(-2);
    init_pmht.remove(3);
    init_pmht.remove(5);
    init_pmht.remove(9);
    init_pmht.remove(10);
    init_pmht.remove(11);
    init_pmht.remove(16);
}

//to see if after an insertion
TEST(PMHT_TEST, InserCorrect) {
    PMHT<10> init_pmht;
    init_pmht.insert(5);
    ASSERT_EQ(init_pmht.size(), 1);
    ASSERT_TRUE( init_pmht.contains(5) );
}