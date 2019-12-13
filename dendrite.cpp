#include "dendrite.h"
#include "random"

Dendrite::Dendrite()
{

}

Dendrite::Dendrite(BitArray& input, float potentialPercent, float maxInitialPerm, float connectedThreshold)
{
    this->connectedThreshold = static_cast<unsigned char>(connectedThreshold * 255);
    unsigned char maxPerm = static_cast<unsigned char>(maxInitialPerm * 255);
    if(potentialPercent >= 0.9999f)
    {
        for(unsigned int i = 0; i < input.size(); i ++)
        {
            unsigned char perm = rand() % maxPerm;
            permanences.push_back(perm);
        }
        potentialIndices = nullptr;
    }
    else
    {
        int numSynapses = static_cast<int>(input.size() * potentialPercent);
    }
}

unsigned short Dendrite::computeOverlap(BitArray *input)
{
    overlap = 0;
    if(!potentialIndices)
    {
        for(unsigned int i = 0; i < input->size(); i ++)
        {
            if(input->at(i) && permanences[i] >= connectedThreshold)
            {
                overlap ++;
            }
        }
    }
    return  overlap;
}

unsigned short Dendrite::getOverlap()
{
    return overlap;
}

BitArray Dendrite::getConnections()
{
    BitArray ret(permanences.size(), 1);
    for(unsigned int i = 0; i < ret.size(); i ++)
    {
        ret[i] = permanences[i] >= connectedThreshold;
    }
    return ret;
}

void Dendrite::fit(BitArray *input, unsigned char inc, unsigned char dec)
{
    if(!potentialIndices)
    {
        for(unsigned int i = 0; i < input->size(); i ++)
        {
            if(input->at(i))
            {
                permanences[i] = std::min(255, permanences[i] + inc);
            }
            else
            {
                permanences[i] = std::max(0, permanences[i] - dec);
            }
        }
    }
}

std::vector<unsigned char> Dendrite::getPermanences()
{
    return permanences;
}
