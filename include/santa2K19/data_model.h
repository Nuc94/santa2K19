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
private:
    //matrix containing families data, one row for each family
    std::array< std::array<int, N_CHOICES>, N_FAMILIES > families_data;
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
        //in this loop I read all lines
        ss << line;
        std::getline(ss, acq, ',');
        family_id = std::stoi(acq);
        choice_level = 0;
        while(std::getline(ss, acq, ',')) {
            this->families_data[family_id][choice_level] = std::stoi(acq);
            ++choice_level;
        }
    }
}

#endif