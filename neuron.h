#ifndef NEURON_H
#define NEURON_H

#include "vector"
#include "dendrite.h"
#include "bitarray.h"

class Neuron
{
    std::vector<Dendrite*> proximal;
    std::vector<Dendrite*> distal;
    std::vector<Dendrite*> apical;

public:
    Neuron();

    Neuron(BitArray& input, float potentialPercent = 1.0f);

    unsigned short computeOverlap(BitArray* input);
    unsigned short getOverlapScore();
    void fitProximal(BitArray* input, unsigned char dendrite = 0, unsigned char inc = 20, unsigned char dec = 10);
    BitArray getProximalConnections(unsigned int dendrite = 0);
};

#endif // NEURON_H
