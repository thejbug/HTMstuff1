#ifndef NEURON_H
#define NEURON_H

#include "vector"
#include "dendrite.h"
#include "bitarray.h"

class Neuron
{

public:

    std::vector<Dendrite*> proximal;
    std::vector<Dendrite*>* distal;
    std::vector<Dendrite*> apical;

    Neuron();

    Neuron(BitArray& input, float potentialPercent = 1.0f);

    unsigned short computeOverlap(BitArray* input);
    unsigned short getOverlapScore();

    void computeDistalOverlap(BitArray* input);
    unsigned int computeActiveDistalDendrites(int threshold);

    void fitProximal(BitArray* input, unsigned char dendrite = 0, unsigned char inc = 20, unsigned char dec = 10);
    void fitDistal(std::vector<BitArray*> input, unsigned char inc = 20, unsigned char dec = 10);
    Dendrite* getProximalDendrite(unsigned int dendrite = 0);
};

#endif // NEURON_H
