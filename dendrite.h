#ifndef DENDRITE_H
#define DENDRITE_H

#include "vector"
#include "bitarray.h"

class Dendrite
{

public:

    unsigned char connectedThreshold;
    std::vector<unsigned short>* potentialIndices;
    std::vector<unsigned char> permanences;

    bool active_;
    unsigned char inputID;
    unsigned short overlap;
    unsigned short potentialOverlap;


    Dendrite();

    Dendrite(BitArray& input, float potentialPercent = 1, float maxInitialPerm = 0.3f, float connectedThreshold = 0.15f);

    static Dendrite* growSegment(BitArray* input, unsigned char id, float initialPerm = 0.5f, float connectedThreshold = 0.35f);

    //computes, stores, and returns the overlap of this dendrite with the input
    unsigned short computeOverlap(BitArray* input);
    //returns the stored overlap
    unsigned short getOverlap();
    //returns the number of potential synapses
    unsigned short receptiveFieldSize();

    BitArray getConnections();
    std::vector<unsigned char> getPermanences();

    void fit(BitArray* input, unsigned char inc, unsigned char dec);
};

#endif // DENDRITE_H
