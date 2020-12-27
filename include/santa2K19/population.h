#ifndef POPULATION_H
#define POPULATION_H

#include "data_model.h"
#include "chromosome.h"

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdlib> //for the random number generation

//I declare in advance the classes I'm going to use

class Population;
class OffspringPolicy;

using evolution_policies = std::vector< std::pair< unsigned int, std::unique_ptr<OffspringPolicy> > >;

using selection_avoid_method_type = int (Population::*)(int) const;
using selection_fill_method = void (Population::*) (std::vector<int> &, const unsigned int) const;
using crossover_policy_type = void (Chromosome::*)(Chromosome &);
using mutation_policy_type = void (Chromosome::*)();

//Then proceed to define Population class

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
    //I need a method to execute an evoulution given a set of policies
    void evolve(const evolution_policies & policies);
    //list of selection functions, they return an int signaling a position
    //in the vector according to a choice for the element to reproduce
    template<int size>
        int tournamentSelect(const int avoid) const {
        int cand, result, least = this->pop_elems->size() - 1;
        if(least == avoid && least > 0) least -= 1;
        result = least;
        for(int i = 0; i < size; ++i) {
            cand = std::rand() % this->pop_elems->size();
            if (cand == avoid) cand = least;
            result = std::min( result, cand );
        }
        return result;
    }
    inline int binaryTournamentSelect(const int avoid) const {
        return this->tournamentSelect<2>(avoid);
    }
    int rankSelect(const int avoid) const;
    //I then need to define methods to actually fill a selection vector of ints
    void fillByElitism( std::vector<int> & sel_target,
                        const unsigned int sel_size) const;
    template<int n>
        void fillByTournamentSelect(    std::vector<int> & sel_target,
                                        const unsigned int sel_size) const {
            this->fillAvoiding(sel_target, sel_size, &Population::tournamentSelect<n>);
        }
    void fillByRankSelect(  std::vector<int> & sel_target,
                            const unsigned int sel_size) const {
        this->fillAvoiding(sel_target, sel_size, &Population::rankSelect);
    }
    //public method to get the weigths for the rank selection procedure
    inline const std::vector<int> & getRankSelWeigths() const { return this->rank_sel_weights; }
private:
    std::unique_ptr< pop_container_type > pop_elems;
    std::vector<int> rank_sel_weights; //This vector is used for rank selection
    int rank_sel_sum;
    //method which sorts the chromosomes in the population in order of performance
    void sortElems();
    //method useful at construction to build the weights for performing rank selection
    void buildRankSelWeightsAndSum(unsigned int pop_size);
    //method to build a selection vector - a vector containing int positions
    //of elements selected to generate next offspring -
    std::vector<int> buildSelectionVector(  const evolution_policies & policies,
                                            const unsigned int sel_dimension) const;
    //with this method we just obtain a vector with the unique pointers to
    //chromosomes at the right positions to then perform crossover and mutation
    std::unique_ptr< pop_container_type > buildNextPopElems(const std::vector<int> & sel_vector) const;
    //method to apply crossover and mutation operators
    void applyPolicyEvolutionOperators(const evolution_policies & policies);
    //private selection method to invoke methods which avoid
    //the previous element selection
    void fillAvoiding(  std::vector<int> & sel_target,
                        const unsigned int sel_size,
                        selection_avoid_method_type sel_method) const;
};

/*  a function useful for rank selection */
int searchLowLim(int low_lim, int up_lim, int search_objective,
                const std::vector<int> & sel_weights);

/*  a function to obtain given some policies the next_pop dimension */
int getNextPopDimension(const evolution_policies & policies);

//And then to define OffspringPolicy

class OffspringPolicy {
public:
    OffspringPolicy(selection_fill_method input_sel_fun) :
                    OffspringPolicy(input_sel_fun, nullptr, nullptr) {};
    OffspringPolicy(selection_fill_method input_sel_fun,
                    crossover_policy_type input_cross_fun,
                    mutation_policy_type input_mut_fun) : sel_fun(input_sel_fun),
                    cross_fun(input_cross_fun), mut_fun(input_mut_fun) {};
    void applySelection(const Population * pop, std::vector<int> & sel_target,
                        const unsigned int sel_size) const;
    void applyEvolutionOperators(const std::unique_ptr< pop_container_type > & pop_elems,
                                const int low_limit, const int up_limit) const;
private:
    const selection_fill_method sel_fun;
    const crossover_policy_type cross_fun;
    const mutation_policy_type mut_fun;
};



#endif