#include "dendrite.h"
#include "random"

Dendrite::Dendrite()
{
    potentialIndices = new std::vector<unsigned short>;
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

using namespace  std;

Dendrite* Dendrite::growSegment(BitArray *input, unsigned char id, float initialPerm, float connectedThreshold)
{
    Dendrite* dend = new Dendrite;
    //dend->potentialIndices = new std::vector<unsigned short>;
    //*dend->potentialIndices = input->onBits();
    dend->potentialIndices = new std::vector<unsigned short>(input->onBits());
    dend->inputID = id;
    dend->permanences = std::vector<unsigned char>(dend->potentialIndices->size());
    dend->connectedThreshold = static_cast<unsigned char>(connectedThreshold * 255);
    unsigned char perm = static_cast<unsigned char>(initialPerm * 255);
    for(unsigned int p = 0; p < dend->permanences.size(); p ++)
    {
        dend->permanences[p] = perm;
    }
    return dend;
}


//computes and stores two overlap scores. one is the overlap with actual connections witht the input,
//the other is the overlap of the permanences with the input. returns the actual overlap
unsigned short Dendrite::computeOverlap(BitArray *input)
{
    overlap = 0;
    potentialOverlap = 0;
    if(!potentialIndices)
    {
        for(unsigned int i = 0; i < input->size(); i ++)
        {
            if(input->at(i))
            {

                if(permanences[i] >= connectedThreshold)
                {
                    overlap ++;
                }
            }
        }
    }
    //if there are indices
    else
    {
        auto on = input->onBits();
        for(unsigned int i = 0; i < potentialIndices->size(); i ++)
        {
            unsigned int index = potentialIndices->at(i);
            if(input->at(index))
            {
                potentialOverlap ++;

                if(permanences[i] >= connectedThreshold)
                {
                    overlap ++;
                }
            }
        }
    }

    return overlap;
}

//returns the stored overlap
unsigned short Dendrite::getOverlap()
{
    return overlap;
}

unsigned short Dendrite::receptiveFieldSize()
{
    return permanences.size();
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
    else
    {
        //fit the existing synapses to the input
        //also track any synapses that need to be deleted
        std::vector<unsigned short> deadSynapses;
        for(unsigned int i = 0; i < potentialIndices->size(); i ++)
        {
            unsigned int index = potentialIndices->at(i);
            if(input->at(index))
            {
                permanences[i] = std::min(255, permanences[i] + inc);
            }
            else
            {
                permanences[i] = std::max(0, permanences[i] - dec);
            }
            if(permanences[i] == 0)
            {
                deadSynapses.push_back(i);
            }
        }
        //grow new synapses if necessary
        //for each on bit...
        auto on = input->onBits();
        for(unsigned int i = 0; i < on.size(); i ++)
        {
            //if it's not already in the potential indices, add the new synapse
            if(BitArray::in(on[i], potentialIndices) < 0)
            {
                potentialIndices->push_back(on[i]);
                permanences.push_back(connectedThreshold);
            }
        }

        //delete dead synapsees
        if(deadSynapses.size() > 0)
        {
            //copy old synapses if they are not dead
            auto newIndices = new std::vector<unsigned short>;
            std::vector<unsigned char> newPerms;
            for(unsigned int i = 0; i < potentialIndices->size(); i ++)
            {
                if(!BitArray::in(i, &deadSynapses))
                {
                    newIndices->push_back(potentialIndices->at(i));
                    newPerms.push_back(permanences[i]);
                }
            }
            delete potentialIndices;
            potentialIndices = newIndices;
            permanences = newPerms;
        }

    }
}

std::vector<unsigned char> Dendrite::getPermanences()
{
    return permanences;
}
