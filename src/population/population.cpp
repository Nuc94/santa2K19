#include "../../include/santa2K19/population.h"

Population::Population(const unsigned int pop_size, const DataModel & input_dm) {
    this->pop_elems = std::make_unique<pop_container_type>();
    this->pop_elems->reserve( pop_size );
    for(int i = 0; i < static_cast<int>(pop_size); ++i) {
        this->pop_elems->emplace_back( std::make_unique<Chromosome>(input_dm) );
    }
    this->buildRankSelWeightsAndSum(pop_size);
    this->sortElems();
}

void Population::evolve(const evolution_policies & policies) {
    int next_pop_dim = getNextPopDimension(policies);
    std::vector<int> sel_vector = this->buildSelectionVector(policies, next_pop_dim);
    std::unique_ptr< pop_container_type > pop_elems = this->buildNextPopElems(sel_vector);
}

void Population::sortElems() {
    std::sort(this->pop_elems->begin(), this->pop_elems->end(),
        []( const std::unique_ptr< Chromosome > & a,
            const std::unique_ptr< Chromosome > & b) -> bool {
                return a->getCost() < b->getCost();
            }
    );
}

/*  this function shall just return a selection according to rank selection criteria */
int Population::rankSelect(const int avoid) const {
    int up_lim, low_lim, split_point, rand_outcome = std::rand() % this->rank_sel_sum;
    up_lim = this->rank_sel_weights.size();
    low_lim = 0;
    low_lim = searchLowLim(low_lim, up_lim, rand_outcome, this->rank_sel_weights);
    if(low_lim == avoid) {
        up_lim = low_lim + 1;
        rand_outcome = std::rand() % this->rank_sel_sum;
        while( rand_outcome >= this->rank_sel_weights[low_lim]
        && rand_outcome < this->rank_sel_weights[up_lim] )
            rand_outcome = std::rand() % this->rank_sel_sum;
        if (rand_outcome >= this->rank_sel_weights[up_lim] ) {
            low_lim = up_lim;
            up_lim = this->rank_sel_weights.size();
        }
        else {
            up_lim = low_lim;
            low_lim = 0;            
        }
        low_lim = searchLowLim(low_lim, up_lim, rand_outcome, this->rank_sel_weights);
    }
    return low_lim;
}

void Population::buildRankSelWeightsAndSum(unsigned int pop_size) {
    this->rank_sel_sum = 0;
    this->rank_sel_weights.reserve( this->pop_elems->size() + 1 );
    this->rank_sel_weights.push_back( this->rank_sel_sum );
    for(int i = 0; i < this->pop_elems->size(); ++i) {
        this->rank_sel_weights.push_back( this->rank_sel_sum += pop_size );
        --pop_size;
    }
}

void Population::fillByElitism( std::vector<int> & sel_target,
                                const unsigned int sel_size) const {
    //this variable is declared to avoid modulo operation
    int sel = 0;
    for(int i = 0; i < static_cast<int>(sel_size); ++i) {
        sel_target.push_back( sel );
        ++sel;
        //Operation executed to avoid modulo operation, while ensuring
        //that an elitist selection doesn't
        if(sel >= this->pop_elems->size()) sel = 0; 
    }
}

//method to build a selection vector - a vector containing int positions
//of elements selected to generate next offspring -
std::vector<int> Population::buildSelectionVector(   const evolution_policies & policies,
                                    const unsigned int sel_dimension) const {
    std::vector<int> sel_result;
    sel_result.reserve(sel_dimension);
    for(int i = 0; i < policies.size(); ++i) {
        policies[i].second->applySelection( this, sel_result, policies[i].first );
    }
    return sel_result;
}

//with this method we just obtain a vector with the unique pointers to
//chromosomes at the right positions to then perform crossover and mutation
std::unique_ptr< pop_container_type > Population::buildNextPopElems(const std::vector<int> & sel_vector) const {
    std::unique_ptr< pop_container_type > next_pop = std::make_unique<pop_container_type>();
    std::vector<int> copies_remaining( this->pop_elems->size(), 0 );
    next_pop->reserve( sel_vector.size() );
    //I start by building the vector telling how many times a certain element
    //has to be copied in the next population
    for(int pos: sel_vector) ++copies_remaining[pos];
    for(int pos: sel_vector) {
        if( copies_remaining[pos] == 1 ) {
            next_pop->push_back(std::move( this->pop_elems->operator[](pos) ));
        }
        else {
            next_pop->push_back( std::make_unique<Chromosome>( *(this->pop_elems->operator[](pos)) ) );
            --copies_remaining[pos];
        }
    }
    return next_pop;
}

void Population::fillAvoiding(  std::vector<int> & sel_target,
                    const unsigned int sel_size,
                    selection_avoid_method_type sel_method) const {
    int avoid, sel;
    for(int i = 0; i < static_cast<int>(sel_size); ++i) {
        if(i % 2 == 0) avoid = -1;
        else avoid = sel;
        sel = (this->*sel_method) (avoid);
        sel_target.push_back(sel);
    }
}



/*  a function useful for rank selection */
int searchLowLim(int low_lim, int up_lim, const int search_objective,
                const std::vector<int> & sel_weights) {
    int split_point;
    while(up_lim - low_lim > 1) {
        split_point = low_lim + ( (up_lim - low_lim) / 2 );
        if( sel_weights[ split_point ] > search_objective ) up_lim = split_point;
        else low_lim = split_point;
    }
    return low_lim;
}

/*  a function to obtain given some policies the next_pop dimension */
int getNextPopDimension(const evolution_policies & policies) {
    int next_pop_dim = 0;
    for(int i = 0; i < policies.size(); ++i) {
        next_pop_dim += policies[i].first;
    }
    return next_pop_dim;
}



//OffspringPolicy methods

void OffspringPolicy::applySelection(const Population * pop, std::vector<int> & sel_target,
                        const unsigned int sel_size) const {
    (pop->*this->sel_fun) (sel_target, sel_size);
}