#ifndef BITARRAY_H
#define BITARRAY_H

#include "vector"
#include "iostream"

enum performanceHint
{
    NONE,
    TIME,
    SPACE
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



public:

    unsigned int size();
    bool at(unsigned int index) const;
    void set(unsigned int index, bool value);
    void clear();

    bool operator[](unsigned int) const;
    Proxy operator[](unsigned int);

    BitArray(const BitArray& other);
    BitArray(unsigned int size = 16, float sparsity = 1.0f, performanceHint hint = NONE);
    ~BitArray();

    void print();
};

#endif // BITARRAY_H
