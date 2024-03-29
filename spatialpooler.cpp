#include "spatialpooler.h"
#include <algorithm>
#include <vector>

SpatialPooler::SpatialPooler()
{

}

SpatialPooler::SpatialPooler(unsigned int size, BitArray& input, float sparsity, float potentialPercent)
{
    size_ = size;
    potentialPercent_ = potentialPercent;
    setSparsity(sparsity);

    for(unsigned int n = 0; n < size; n ++)
    {
        Neuron* neuron = new Neuron(input, potentialPercent);
        neurons_.push_back(neuron);
    }

    activity_ = new BitArray(neurons_.size(), 1, SPARSE);

    synapseIncrement_ = 10;
    synapseDecrement_ = 12;
}


void SpatialPooler::setLearningEnabled(bool b)
{
    learningEnabled_ = b;
}
void SpatialPooler::setBoostingEnabled(bool b)
{
    boostingEnabled = b;
}
void SpatialPooler::setSynapseIncrement(float inc)
{
    synapseIncrement_ = static_cast<unsigned char>(inc * 255);
}
void SpatialPooler::setSynapseDecrement(float dec)
{
    synapseDecrement_ = static_cast<unsigned char>(dec * 255);
}
void SpatialPooler::setSynapseConnectedThreshold(float thresh)
{
    synapseConnectedThreshold_ = static_cast<unsigned char>(thresh * 255);
}
void SpatialPooler::setSparsity(float s)
{
    sparsity_ = std::min(1.0f, std::max(0.0f, s));
    numOnBits_ = static_cast<unsigned int>(sparsity_ * size_);
}
void SpatialPooler::setKwinners(unsigned int k)
{
    numOnBits_ = k;
    sparsity_ = static_cast<float>(k) / size_;
}

void SpatialPooler::computeOverlap(BitArray* input)
{
    overlaps = std::vector<NeuronScore>();//(neurons.size());
    for(unsigned short i = 0; i < neurons_.size(); i ++)
    {
        Neuron* n = neurons_[i];
        int overlap = n->computeOverlap(input);
        //std::cout << "col " << i << ": " << overlap << "\n";
        overlaps.push_back(NeuronScore(i, n->computeOverlap(input)));
    }
    //std::cout << std::endl;
}
void SpatialPooler::computeKWinners()
{
    activity_->clear();
    std::sort(overlaps.begin(), overlaps.end());
    for(unsigned short i = 0; i < numOnBits_; i ++)
    {
        unsigned short indexInOverlapList = neurons_.size() - 1 - i;
        unsigned short indexOfSpBit = overlaps[indexInOverlapList].index;
        activity_->set(indexOfSpBit, true);
    }
}
BitArray* SpatialPooler::activity()
{
    return activity_;
}

std::vector<Neuron*> SpatialPooler::neurons()
{
    return neurons_;
}

void SpatialPooler::fit(BitArray *input)
{
    for(unsigned int i = overlaps.size() - 1; i > overlaps.size() - numOnBits_ - 1; i --)
    {
        int neuronIndex = overlaps[i].index;
        neurons_[neuronIndex]->fitProximal(input, 0, synapseIncrement_, synapseDecrement_);
    }
}

unsigned int SpatialPooler::size()
{
    return size_;
}

