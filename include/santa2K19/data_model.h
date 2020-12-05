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

#endif