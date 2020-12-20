#ifndef POPULATION_H
#define POPULATION_H

#include "data_model.h"
#include "chromosome.h"

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

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
private:
    std::unique_ptr< pop_container_type > pop_elems;
    void sortElems();
};



#endif