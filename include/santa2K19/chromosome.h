#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include "data_model.h"
#include "pmht.h"

#include <array>
#include <cstdlib> //for the random number generation and abs
#include <boost/container/flat_set.hpp>

class Chromosome {
public:
    //I need a default constructor using random numbers
    Chromosome(const DataModel & input_dm);
    inline double getCost() const { return this->families_cost + this->accounting_cost + this->overshooting_penalty; }
    void elementaryMutation(const int family_id, const int new_day);
    void singlePointCrossover(Chromosome & other, int crossover_point);
private:
    double families_cost;
    double accounting_cost;
    double overshooting_penalty;
    std::array<int, N_FAMILIES> families_assignments;
    std::array<int, N_FAMILIES> choice_level;
    std::array<int, N_DAYS> daily_occupancy;
    PMHT<N_DAYS> overshooting_days;
    const DataModel & dm;
    void randomizeChromosome();
    void randomizeAssignments();
    void resetDailyOccupancy();
    void countDailyOccupancy();
    void calculateCost();
    void calculateFamiliesCost();
    void calculateAccoutingCost();
    double getAccountingCostsAtDay(const int day) const;
    void buildOvershootingDaysSet();
    inline void updateOveshootingPenalty() {
        if( this->overshooting_days.size() == 0 ) overshooting_penalty = 0;
        else this->overshooting_penalty = OVERSHOOT_PENALTY;
    }
};

#endif