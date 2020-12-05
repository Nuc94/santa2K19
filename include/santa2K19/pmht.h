#ifndef PMHT_H
#define PMHT_H

#include <array>

template<int LEN>
class PMHT {
public:
    //method to obtain the number elements contained in the pmht
    inline int size() const { return this->elements_contained; }
    inline bool contains(int element) const {
        if( element < 0 || element >= LEN ) {
            return false;
        }
        return this->inner_ht[element];
    }
    inline void insert(int element) {
        if( element >= 0 && element < LEN ) {
            if(! this->inner_ht[element]) {
                ++this->elements_contained;
                this->inner_ht[element] = true;
            }
        }
    }
    inline void remove(int element) {
        if( element >= 0 && element < LEN ) {
            if(this->inner_ht[element]) {
                --this->elements_contained;
                this->inner_ht[element] = false;
            }
        }
    }
    //Class constructor
    PMHT() : elements_contained{0} {}
private:
    int elements_contained;
    std::array<bool, LEN> inner_ht;   
};

#endif