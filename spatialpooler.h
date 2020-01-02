#ifndef SPATIALPOOLER_H
#define SPATIALPOOLER_H

#include "bitarray.h"
#include "neuron.h"

class SpatialPooler
{
    //stored values
    BitArray* activity_;

    std::vector<Neuron*> neurons_;
    struct NeuronScore
    {
    public:
        unsigned short index;
        unsigned short score;

        NeuronScore(unsigned short id, unsigned short s): index(id), score(s) {}

        bool operator<(const NeuronScore ns) const
        {
            return score < ns.score;
        }
    };
    std::vector<NeuronScore> overlaps;

public:

    unsigned int size_;
    float sparsity_;
    unsigned short numOnBits_;

    float potentialPercent_;

    bool boostingEnabled;
    float maxBoostFactor_;
    float minBoostFactor_;

    //learning parameters
    bool learningEnabled_;
    unsigned char synapseIncrement_ = 20;
    unsigned char synapseDecrement_ = 8;
    unsigned char synapseConnectedThreshold_ = 50;

    SpatialPooler();

    SpatialPooler(unsigned int size, BitArray& input, float sparsity = 0.05f, float potentialPercent = 1.0f);

    unsigned int size();
    void setLearningEnabled(bool b);
    void setBoostingEnabled(bool b);
    void setSynapseIncrement(float inc);
    void setSynapseDecrement(float dec);
    void setSynapseConnectedThreshold(float thresh);
    void setSparsity(float s);
    void setKwinners(unsigned int k);
    std::vector<Neuron*> neurons();

    void computeOverlap(BitArray* input);
    void computeKWinners();
    BitArray* activity();
    void fit(BitArray* input);
};

#endif // SPATIALPOOLER_H
