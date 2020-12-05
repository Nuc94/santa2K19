#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#include<string>
#include<fstream>
#include<sstream>
#include<array>

//PROBLEM CONSTANTS

const int N_FAMILIES = 5000;
const int N_DAYS = 100;
const int N_CHOICES = 10;

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
private:
    //matrix containing families choices, one row for each family
    std::array< std::array<int, N_CHOICES>, N_FAMILIES > families_choices;
    //array containing number of components for each family
    std::array<int, N_FAMILIES> families_components;
    //internal method to parse families file
    void parseFamiliesCsv(std::ifstream & families_file);
};

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
            this->families_choices[family_id][choice_level] = std::stoi(acq);
            ++choice_level;
        }
        std::getline(ss, acq);
        this->families_components[family_id] = std::stoi(acq);
        ss.str("");
        ss.clear();
    }
}

#endif