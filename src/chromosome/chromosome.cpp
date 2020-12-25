#include "../../include/santa2K19/chromosome.h"

Chromosome::Chromosome(const DataModel & input_dm) : dm(input_dm) {
    this->randomizeChromosome();
}

void Chromosome::randomizeChromosome() {
    this->randomizeAssignments();
    this->countDailyOccupancy();
    this->buildOvershootingDaysSet();
    this->calculateCost();
}

void Chromosome::randomizeAssignments() {
    //std::srand(time(0));
    for(uint i = 0; i < this->families_assignments.size(); ++i) {
        this->families_assignments[i] = std::rand() % N_DAYS;        
    }
}

void Chromosome::resetDailyOccupancy() {
    for(uint i = 0; i < this->daily_occupancy.size(); ++i) {
        this->daily_occupancy[i] = 0;
    }
}

void Chromosome::countDailyOccupancy() {
    this->resetDailyOccupancy();
    for(uint i = 0; i < this->families_assignments.size(); ++i) {
        ++this->daily_occupancy[this->families_assignments[i] ];
    }
}

void Chromosome::calculateCost() {
    this->calculateFamiliesCost();
    this->calculateAccoutingCost();
    this->updateOveshootingPenalty();
}

void Chromosome::calculateFamiliesCost() {
    this->families_cost = 0.0;
    for(int family_id = 0; family_id < this->families_assignments.size(); ++family_id) {
        this->families_cost += dm.getFamilyCostAtDay(family_id, this->families_assignments[family_id]);
    }
}

void Chromosome::calculateAccoutingCost() {
    this->accounting_cost = 0.0;
    int diff;
    for(int day = 0; day < this->daily_occupancy.size() - 1; ++day) {
        diff = std::abs( this->daily_occupancy[day] - this->daily_occupancy[day + 1] );
        this->accounting_cost += dm.getAccoutingCost(this->daily_occupancy[day], diff);
    }
    this->accounting_cost += dm.getAccoutingCost(this->daily_occupancy[this->daily_occupancy.size() - 1], 0);
}

double Chromosome::getAccountingCostsAtDay(const int day) const {
    int diff;
    if(day == N_DAYS - 1) diff = 0;
    else diff = std::abs( this->daily_occupancy[day] - this->daily_occupancy[day + 1] );
    return dm.getAccoutingCost(this->daily_occupancy[day], diff);
}

void Chromosome::buildOvershootingDaysSet() {
    this->overshooting_days.reset();
    for(int day = 0; day <= this->daily_occupancy.size(); ++day) {
        if(! legalOccupancy( this->daily_occupancy[day] )) {
            this->overshooting_days.insert(day);
        }
    }
}

void Chromosome::elementaryMutation(const int family_id, const int new_day) {
    int old_day;
    int diff;
    int family_size;
    boost::container::flat_set<int> impacted_accounting_days;
    //I execute the mutation only if the family id and the day are valid
    if (family_id >= 0 && family_id < this->families_assignments.size() && new_day >= 0 && new_day < N_DAYS) {
        //at the beginning I obtain the old day choice from the assignments array
        old_day = this->families_assignments[family_id];
        //then I update the family costs
        this->families_cost -= this->dm.getFamilyCostAtDay(family_id, old_day);
        this->families_cost += this->dm.getFamilyCostAtDay(family_id, new_day);
        //then I proceed to derive the days which are going to be impacted by the mutation
        //regarding the accounting costs
        impacted_accounting_days.reserve(4);
        if(old_day - 1 >= 0) impacted_accounting_days.insert(old_day - 1);
        impacted_accounting_days.insert(old_day);
        if(new_day - 1 >= 0) impacted_accounting_days.insert(new_day - 1);
        impacted_accounting_days.insert(new_day);
        //then for each impacting day I remove its accounting cost given
        //the still unupdated daily occupancy array
        for(int day : impacted_accounting_days) {
            this->accounting_cost -= this->getAccountingCostsAtDay(day);
        }
        //I then proceed to update the daily occupancy array, taking care of
        //eventual penalties for violating the constraints by updating
        //the set of overhooting days accordingly
        family_size = this->dm.getFamilyComponents(family_id);
        this->daily_occupancy[old_day] -= family_size;
        if( legalOccupancy( this->daily_occupancy[old_day] ) ) this->overshooting_days.remove(old_day);
        else this->overshooting_days.insert(old_day);
        this->daily_occupancy[new_day] -= family_size;
        if( legalOccupancy( this->daily_occupancy[new_day] ) ) this->overshooting_days.remove(new_day);
        else this->overshooting_days.insert(new_day);
        //then I proceed to update the accounting costs and the penalty ones
        for(int day : impacted_accounting_days) {
            this->accounting_cost += this->getAccountingCostsAtDay(day);
        }
        this->updateOveshootingPenalty();
        //and finally I assign the new family to the given day
        this->families_assignments[family_id] = new_day;
    }
}

void Chromosome::singlePointCrossover(Chromosome & other, int crossover_point) {
    int copy_support;
    int lower_range, upper_range;
    if(crossover_point > N_FAMILIES - 2) crossover_point = N_FAMILIES - 2;
    if(crossover_point > N_FAMILIES / 2) {
        lower_range = crossover_point + 1;
        upper_range = N_FAMILIES;
    } else {
        lower_range = 0;
        upper_range = crossover_point + 1;
    }
    for(int i = lower_range; i < upper_range; ++i) {
        this->daily_occupancy[ this->families_assignments[i] ] -= this->dm.getFamilyComponents(i);
        other.daily_occupancy[ other.families_assignments[i] ] -= this->dm.getFamilyComponents(i);
        this->daily_occupancy[ other.families_assignments[i] ] += this->dm.getFamilyComponents(i);
        other.daily_occupancy[ this->families_assignments[i] ] += this->dm.getFamilyComponents(i);
        copy_support = this->families_assignments[i];
        this->families_assignments[i] = other.families_assignments[i];
        other.families_assignments[i] = copy_support;
        
    }
    this->buildOvershootingDaysSet();
    this->calculateCost();
    other.buildOvershootingDaysSet();
    other.calculateCost();
}