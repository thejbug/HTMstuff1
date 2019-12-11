#ifndef DENDRITE_H
#define DENDRITE_H

#include "vector"
#include "bitarray.h"

class Dendrite
{
    unsigned char inputID; //maybe not
    bool active_;
    unsigned short overlap;
    unsigned char connectedThreshold;
    std::vector<unsigned short>* potentialIndices;
    std::vector<unsigned char> permanences;

public:
    Dendrite();

    Dendrite(BitArray& input, float potentialPercent = 1, float maxInitialPerm = 0.3f, float connectedThreshold = 0.15f);

    unsigned short computeOverlap(BitArray* input);
    unsigned short getOverlap();
    BitArray getConnections();
    void fit(BitArray* input, unsigned char inc, unsigned char dec);
};

#endif // DENDRITE_H
