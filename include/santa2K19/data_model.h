#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#include<string>
#include<fstream>
#include<sstream>
#include<array>
#include<unordered_map>
#include <utility>
#include <boost/functional/hash.hpp>
#include <math.h> //for power at penalty calculation

//advanced declarations

struct  pair_hash;


//PROBLEM CONSTANTS

const int N_FAMILIES = 5000;
const int N_DAYS = 100;
const int N_CHOICES = 10;
const double OVERSHOOT_PENALTY = 1000000.0; //penalty added to a solution if
                                            //the limitation constraints on
                                            //minimum and maximum occupancy
                                            //per day are violated

class DataModel
{
public:
    DataModel(const std::string & family_data_filename);
    //method to obtain the chosen day for a given family and its choice
    //if the family or the choice exceed the data ranges just return -1
    inline int getFamilyChoice(const unsigned int family_id, const unsigned int choice) const {
        if( family_id < this->families_choices.size()) {
            if( choice < this->families_choices[family_id].size() ) {
                return this->families_choices[family_id][choice];
            }
        }
        return -1;
    }
    inline int getFamilyComponents(const unsigned int family_id) const {
        if( family_id < this->families_choices.size()) {
            return this->families_components[family_id];
        }
        return -1;
    }
    inline int getFamilyDayChoiceLevel(const int family_id, const int day) const {
        std::unordered_map<std::pair<int, int>, int, boost::hash<std::pair<int, int>>>::const_iterator it = this->choice_level.find( std::make_pair(family_id, day) );
        if(it == this->choice_level.end()) return 10;
        return it->second;
    }
    int getFamilyCostAtDay(const int family_id, const int day) const;
    double getAccoutingCost(const int occupancy, const int diff_with_prev_day) const;
private:
    //matrix containing families choices, one row for each family
    std::array< std::array<int, N_CHOICES>, N_FAMILIES > families_choices;
    //array containing number of components for each family
    std::array<int, N_FAMILIES> families_components;
    //hash table containing the choice level for a given pair family_id - day
    std::unordered_map<std::pair<int, int>, int, boost::hash<std::pair<int, int>>> choice_level;
    std::array< std::array<double, 176>, 176 > penaltyByoccupancyAndDiff;
    //internal method to parse families file
    void parseFamiliesCsv(std::ifstream & families_file);
    void buildChoiceLevels();
    void buildPenalties();
};

#endif