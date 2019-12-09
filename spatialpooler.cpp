#include "spatialpooler.h"

SpatialPooler::SpatialPooler()
{

}

SpatialPooler::SpatialPooler(unsigned int size, float sparsity)
{
    size_ = size;
    setSparsity(sparsity);

}


void SpatialPooler::setLearningEnable(bool b)
{
    learningEnabled_ = b;
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

//void computeOverlap(BitArray& input);
//void computeKWinners();
