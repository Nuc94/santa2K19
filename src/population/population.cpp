#include "../../include/santa2K19/population.h"

Population::Population(const unsigned int pop_size, const DataModel & input_dm) {
    this->pop_elems = std::make_unique<pop_container_type>();
    this->pop_elems->reserve( pop_size );
    for(int i = 0; i < static_cast<int>(pop_size); ++i) {
        this->pop_elems->emplace_back( std::make_unique<Chromosome>(input_dm) );
    }
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