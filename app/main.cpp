#include <iostream>
#include "../include/santa2K19/population.h"

int main() {

    std::cout << "This executable starts here" << std::endl;

    const std::string DEFAULT_FILEPATH = "/home/nuc/Programmazione/Kaggle/Santa's Workshop Tour 2019/family_data.csv";

    const int pop_size = 1000;
    const int n_gen = 1000;
    const int elite_size = 10;

    evolution_policies ev_pol;

    ev_pol.push_back( std::make_pair(elite_size, std::make_unique<OffspringPolicy>( &Population::fillByElitism) ) );
    ev_pol.push_back( std::make_pair(elite_size, std::make_unique<OffspringPolicy>( &Population::fillByElitism, &Chromosome::randomSinglePointCrossover, &Chromosome::randomElementaryMutation ) ) );
    ev_pol.push_back( std::make_pair(pop_size - 2 * elite_size, std::make_unique<OffspringPolicy>( &Population::fillByTournamentSelect<2>, &Chromosome::randomSinglePointCrossover, &Chromosome::randomElementaryMutation ) ) );

    DataModel DM(DEFAULT_FILEPATH);
    Population Pop(pop_size,DM);
    for(int i = 0; i < n_gen; ++i) {
        Pop.evolve(ev_pol);
        std::cout << static_cast<int>(Pop.getFitnessAt(0)) << std::endl;
    }
    
}