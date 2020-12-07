#include "../../include/santa2K19/data_model.h"

DataModel::DataModel(const std::string & family_data_filename)
{
    //In here there shall be the code to effectively parse the .csv
    std::ifstream families_file;
    families_file.open(family_data_filename);
    this->parseFamiliesCsv(families_file);
    this->buildChoiceLevels();
    this->buildPenalties();
}


void DataModel::parseFamiliesCsv(std::ifstream & families_file) {
    std::string line, acq;
    std::stringstream ss;
    int family_id, choice_level;
    //I read and throw away the first line containing the intestatin of the csv
    std::getline(families_file, line);
    //NOW I HAVE TO WRITE CODE TO ACTUALLY GET ALL THE LINES IN A WHILE LOOP AND PARSE EACH ONE INTO THE ARRAY
    while( std::getline(families_file, line) ) {
        //in this loop I read all lines, for each of them I collect data related to the given family
        ss << line;
        std::getline(ss, acq, ',');
        family_id = std::stoi(acq);
        choice_level = 0;
        while(choice_level < N_CHOICES) {
            std::getline(ss, acq, ',');
            this->families_choices[family_id][choice_level] = std::stoi(acq) - 1;
            ++choice_level;
        }
        std::getline(ss, acq);
        this->families_components[family_id] = std::stoi(acq);
        ss.str("");
        ss.clear();
    }
}

void DataModel::buildChoiceLevels() {
    for(int family_id = 0; family_id < this->families_choices.size(); ++family_id) {
        for(int choice_level = 0; choice_level < this->families_choices[family_id].size(); ++choice_level) {
            this->choice_level.insert( std::make_pair( std::make_pair(family_id, this->families_choices[family_id][choice_level]) , choice_level ) );
        }
    }
}

void DataModel::buildPenalties() {
    double penalty;
    for(int occupancy = MINIMUM_OCCUPANCY; occupancy <= MAXIMUM_OCCUPANCY; ++occupancy) {
        for(int diff = 0; diff <= 175; ++diff) {
            penalty = (occupancy - 125) / 400;
            penalty *= pow( occupancy, 0.5 + (diff / 50) );
            this->penaltyByoccupancyAndDiff[occupancy - MINIMUM_OCCUPANCY][diff] = penalty;
        }
    }
}

int DataModel::getFamilyCostAtDay(const int family_id, const int day) const {
    int choice_level = this->getFamilyDayChoiceLevel(family_id, day);
    int family_cost = 0;
    int family_size = this->getFamilyComponents(family_id);
    switch (choice_level) {
        case 0:
            break;
        case 1:
            family_cost += 50;
            break;
        case 2:
            family_cost += 50;
            family_cost += family_size * 9;
            break;
        case 3:
            family_cost += 100;
            family_cost += family_size * 9;
            break;
        case 4:
            family_cost += 200;
            family_cost += family_size * 9;
            break;
        case 5:
            family_cost += 200;
            family_cost += family_size * 18;
            break;
        case 6:
            family_cost += 300;
            family_cost += family_size * 18;
            break;
        case 7:
            family_cost += 300;
            family_cost += family_size * 36;
            break;
        case 8:
            family_cost += 400;
            family_cost += family_size * 36;
            break;
        case 9:
            family_cost += 500;
            family_cost += family_size * (36 + 199);
            break;
        default:
            family_cost += 500;
            family_cost += family_size * (36 + 398);
            break;
    }
    return family_cost;
}

double DataModel::getAccoutingCost(const int occupancy, const int diff_with_prev_day) const {
        if(legalOccupancy(occupancy) && diff_with_prev_day >= 0 && diff_with_prev_day <= 175)
            return this->penaltyByoccupancyAndDiff[occupancy - 125][diff_with_prev_day];
        else return 0.0;
    }