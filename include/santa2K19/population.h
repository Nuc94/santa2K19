#ifndef POPULATION_H
#define POPULATION_H

#include "data_model.h"
#include "chromosome.h"

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdlib> //for the random number generation

using pop_container_type = std::vector< std::unique_ptr< Chromosome > >;

class Population {
public:
    /*  a default constructor just takes in input the dimension of the population
        and generates a random population starting from it */
    Population(const unsigned int pop_size, const DataModel & input_dm);
    inline double getFitnessAt(const unsigned int pos) const {
        if(pos >= this->pop_elems->size()) return -1;
        return this->pop_elems->operator[](pos)->getCost();
    }
    inline unsigned int size() const { return this->pop_elems->size(); }
    //list of selection functions, they return an int signaling a position
    //in the vector according to a choice for the element to reproduce
    template<int size>
        int tournamentSelect(const int avoid) const;
    int rankSelect(const int avoid) const;
    inline const std::vector<int> & getRankSelWeigths() const { return this->rank_sel_weights; }
private:
    std::unique_ptr< pop_container_type > pop_elems;
    std::vector<int> rank_sel_weights;
    int rank_sel_sum;
    void sortElems();
    void buildRankSelWeightsAndSum(unsigned int pop_size);
};

/*  a function useful for rank selection */
int searchLowLim(int low_lim, int up_lim, int search_objective,
                const std::vector<int> & sel_weights);

#endif