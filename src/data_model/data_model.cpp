#include "../../include/santa2K19/data_model.h"

DataModel::DataModel(const std::string & family_data_filename)
{
    //In here there shall be the code to effectively parse the .csv
    std::ifstream families_file;
    families_file.open(family_data_filename);
    this->parseFamiliesCsv(families_file);
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
    this->buildChoiceLevels();
}

void DataModel::buildChoiceLevels() {
    for(int family_id = 0; family_id < this->families_choices.size(); ++family_id) {
        for(int choice_level = 0; choice_level < this->families_choices[family_id].size(); ++choice_level) {
            this->choice_level.insert( std::make_pair( std::make_pair(family_id, this->families_choices[family_id][choice_level]) , choice_level ) );
        }
    }
}