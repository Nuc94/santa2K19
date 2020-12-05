#include <iostream>
#include "../include/santa2K19/pmht.h"

int main() {

    std::cout << "This executable starts here" << std::endl;

    PMHT<70> ht;

    if( ht.size() == 0 ) {
        std::cout << std::endl << "cool" << std::endl;
    } 

}