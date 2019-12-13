#include "neuron.h"

Neuron::Neuron()
{

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

void Neuron::fitProximal(BitArray *input, unsigned char dendrite, unsigned char inc, unsigned char dec)
{
    Dendrite* dend = proximal[dendrite];
    dend->fit(input, inc, dec);
}

Dendrite* Neuron::getProximalDendrite(unsigned int dendrite)
{
    return proximal[dendrite];
}

