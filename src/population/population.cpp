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



//OffspringPolicy methods

void OffspringPolicy::applySelection(const Population * pop, std::vector<int> & sel_target,
                        const unsigned int sel_size) const {
    int avoid, sel;
    for(int i = 0; i < static_cast<int>(sel_size); ++i) {
        if(i % 2 == 0) avoid = -1;
        else avoid = sel;
        sel = (pop->*this->sel_fun) (avoid);
        sel_target.push_back(sel);
    }
}