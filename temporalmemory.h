#ifndef TEMPORALMEMORY_H
#define TEMPORALMEMORY_H

#include "spatialpooler.h"

class TemporalMemory
{
    unsigned int height_;


    BitArray* prevActive;
    BitArray* prevPredicted;
    BitArray* prevWinners;

    BitArray* active_;
    BitArray* predicted_;
    BitArray* winners_;

    unsigned int SAMPLE_SIZE;
    int PERMANENCE_INCREMENT;
    int PERMANENCE_DECREMENT;
    int PREDICTED_DECREMENT;
    unsigned char CONNECTED_PERMANENCE;
    unsigned char ACTIVE_THRESHOLD;
    unsigned char LEARNING_THRESHOLD;
    unsigned char INITIAL_PERMANENCE;

public:
    std::vector<Neuron*> neurons;
    SpatialPooler* sp_;

    BitArray* burstingColumns;

    TemporalMemory();

    TemporalMemory(SpatialPooler* sp, unsigned int height = 8);

    void setup(SpatialPooler* sp, unsigned int height = 8);

    void computePredictions(BitArray* distalInput);
    void computeFeedForward(BitArray* input);
    void fitTM(BitArray* distalActive, BitArray* distalWinners);
    void fitDistalDendrite(Dendrite* dend, BitArray* prevActive, BitArray* prevWinners);

    BitArray* active(int t = 0);
    BitArray* prediced(int t = 0);
    BitArray* winners(int t = 0);

    void step();

    unsigned int width();
    unsigned int height();
    unsigned int size();

    unsigned int countSynapses();

    void print();
};

#endif // TEMPORALMEMORY_H
