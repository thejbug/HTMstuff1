#include "neuron.h"

Neuron::Neuron()
{
    distal = new std::vector<Dendrite*>;
}

Neuron::Neuron(BitArray& input, float potentialPercent)
{
    proximal = std::vector<Dendrite*>();
    Dendrite* prox = new Dendrite(input, potentialPercent);
    proximal.push_back(prox);
}

unsigned short Neuron::computeOverlap(BitArray *input)
{
    //assume only one proximal dendrite for now
    for(unsigned int i = 0; i < proximal.size(); i ++)
    {
        proximal[i]->computeOverlap(input);
    }
    return getOverlapScore();
}

unsigned short Neuron::getOverlapScore()
{
    unsigned short overlap = 0;
    for(unsigned int i = 0; i < proximal.size(); i ++)
    {
        overlap += proximal[i]->getOverlap();
    }
    return overlap;
}

void Neuron::computeDistalOverlap(BitArray * input)
{
    for(unsigned int d = 0; d < distal->size(); d ++)
    {
        Dendrite* dend = distal->at(d);
        dend->computeOverlap(input);
    }
}

unsigned int Neuron::computeActiveDistalDendrites(int threshold)
{
    unsigned int numActive = 0;
    for(unsigned int d = 0; d < distal->size(); d ++)
    {
        Dendrite* dend = distal->at(d);
        if(dend->getOverlap() >= threshold)
        {
            dend->active_ = true;
            numActive ++;
        }
        else
        {
            dend->active_ = false;
        }
    }
    return numActive;
}

void Neuron::fitProximal(BitArray *input, unsigned char dendrite, unsigned char inc, unsigned char dec)
{
    Dendrite* dend = proximal[dendrite];
    dend->fit(input, inc, dec);
}

void Neuron::fitDistal(std::vector<BitArray*> input, unsigned char inc, unsigned char dec)
{
    //grow segments to each input block seperately
    for(unsigned char i = 0; i < input.size(); i ++)
    {
        BitArray* block = input[i];
        auto on = block->onBits();
        //loop through all the existing segments to this input block and fit them if they were active
        bool predicted = false;
        for(unsigned int d = 0; d < distal->size(); d ++)
        {
            Dendrite* dend = distal->at(d);
            //if a dendrite is active AND it connects to this block
            if(dend->active_ && dend->inputID == i)
            {
                predicted = true;
                //fit the synapses by adjusting permanences
                dend->fit(input[dend->inputID], inc, dec);
            }
        }
        //if none were active, grow a new segment to this block
        //don't grow a segment if there are no active input bits
        if(!predicted && on.size() > 0)
        {
            Dendrite* newDend = Dendrite::growSegment(block, i);
            distal->push_back(newDend);
        }
    }

}

Dendrite* Neuron::getProximalDendrite(unsigned int dendrite)
{
    return proximal[dendrite];
}

