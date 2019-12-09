#ifndef BITARRAY_H
#define BITARRAY_H

#include "vector"
#include "iostream"
#include "map"

enum performanceHint
{
    NONE,
    DENSE,
    SPARSE
};

class BitArray;

class Proxy
{
    BitArray* array;
    unsigned int index;
public:
    Proxy(BitArray* arr, unsigned int id): array(arr), index(id){}

    Proxy& operator=(bool);
    operator bool();
};

class BitArray
{
    unsigned int size_;
    std::vector<unsigned short> * onBits_;
    unsigned char * bits_;
    performanceHint hint_;
    float expectedSparsity_;

public:

    unsigned int size();

    ///get the value at an index
    bool at(unsigned int index) const;

    ///set the value at an index
    void set(unsigned int index, bool value);

    ///set all bits to 0
    void clear();

    ///set all bits in the interval [start, end] to a single value
    void setRange(int start, int end, bool value);

    ///lists the indices of the on bits
    std::vector<unsigned short> onBits();

    bool operator[](unsigned int) const;

    ///get or set the value at an index
    Proxy operator[](unsigned int);

    ///subset the array with a list of indices
    BitArray operator[](const std::vector<unsigned short>& indices);

    BitArray operator|(BitArray&);

    BitArray(const BitArray& other);
    BitArray(unsigned int size = 16, float expectedSparsity = 1.0f, performanceHint hint = NONE);
    ~BitArray();

    void print();
};


#endif // BITARRAY_H
