#include "temporalmemory.h"
#include "iostream"

TemporalMemory::TemporalMemory()
{

}

TemporalMemory::TemporalMemory(SpatialPooler*, unsigned int)
{
}

unsigned int TemporalMemory::width()
{
    return sp_->size();
}
unsigned int TemporalMemory::height()
{
    return  height_;
}
unsigned int TemporalMemory::size()
{
    return static_cast<unsigned int>(neurons.size());
}

void TemporalMemory::setup(SpatialPooler *sp, unsigned int height)
{
    height_ = height;
    sp_ = sp;
    unsigned int numCells = sp->size() * height;
    neurons = std::vector<Neuron*>(numCells);
    for(unsigned int i = 0; i < numCells; i ++)
    {
        neurons[i] = new Neuron;
    }

    SAMPLE_SIZE = sp->numOnBits_ * 1.0f;

    CONNECTED_PERMANENCE = 100;
    ACTIVE_THRESHOLD = SAMPLE_SIZE * 0.8f;
    INITIAL_PERMANENCE = 95;
    LEARNING_THRESHOLD = 0;
    PERMANENCE_INCREMENT = 10;
    PERMANENCE_DECREMENT = 5;
    PREDICTED_DECREMENT = 3;

    predicted_ = new BitArray(numCells, 0, SPARSE);
    active_ = new BitArray(numCells, 0, SPARSE);
    winners_ = new BitArray(numCells, 0, SPARSE);
    burstingColumns = new BitArray(sp->size(), 0, SPARSE);

    prevPredicted = new BitArray(numCells, 0, SPARSE);
    prevActive = new BitArray(numCells, 0, SPARSE);
    prevWinners = new BitArray(numCells, 0, SPARSE);
}

void TemporalMemory::computePredictions(BitArray* distalInput)
{
    //for each neuron
    for(unsigned int n = 0; n < neurons.size(); n ++)
    {
        //the neuron is predicted if any of it's distal segments are active
        for(unsigned int d = 0; d < neurons[n]->distal->size(); d ++)
        {
            Dendrite* dend = neurons[n]->distal->at(d);
            //count the actual overlap and the potentail overlap (almost connected synapses)
            dend->overlap = 0;
            dend->potentialOverlap = 0;
            //for each synapse
            for(unsigned int s = 0; s < dend->potentialIndices->size(); s ++)
            {
                unsigned int inputIndex = dend->potentialIndices->at(s);
                if(distalInput->at(inputIndex))
                {
                    dend->potentialOverlap ++;
                    if(dend->permanences[s] >= CONNECTED_PERMANENCE)
                    {
                        dend->overlap ++;
                    }
                }
            }

            //a neuron is predicited if it has an active dendrite
            if(dend->overlap >= ACTIVE_THRESHOLD)
            {
                predicted_->set(n, true);
            }
        }
    }
}

void TemporalMemory::computeFeedForward(BitArray *input)
{
    sp_->computeOverlap(input);
    sp_->computeKWinners();
    BitArray* activeCols = sp_->activity();

    //for each minicolumn
    for(unsigned int col = 0; col < sp_->size(); col ++)
    {
        //we only care about columns activated by the spatial pooler
        if(activeCols->at(col))
        {
            bool predictedCol = false;
            //for each cell in the minicolumn
            for(unsigned int cell = 0; cell < height_; cell ++)
            {
                unsigned int index = col * height_ + cell;
                //if a cell is predicted, that cell will be active and the column will not burst
                if(predicted_->at(index))
                {
                    predictedCol = true;
                    active_->set(index, true);
                }
            }

            //if no cells were predicted, the column will burst
            if(!predictedCol)
            {
                burstingColumns->set(col, true);
                for(unsigned int cell = 0; cell < height_; cell ++)
                {
                    unsigned int index = col * height_ + cell;
                    active_->set(index, true);
                }
            }

        }

    }
}

void TemporalMemory::fitDistalDendrite(Dendrite *dend, BitArray *prevActive, BitArray *prevWinners)
{
    //for each existing synapse, adjust permanences based on activity
    //keep track of synapses that have reached zero permanence
    std::vector<unsigned short> deadSynapses;
    for(unsigned int s = 0; s < dend->potentialIndices->size(); s ++)
    {
        unsigned int inputIndex = dend->potentialIndices->at(s);
        if(prevActive->at(inputIndex))
        {
            dend->permanences[s] = std::min(255, dend->permanences[s] + PERMANENCE_INCREMENT);
        }
        else
        {
            dend->permanences[s] = std::max(0, dend->permanences[s] - PERMANENCE_DECREMENT);
        }

        if(dend->permanences[s] == 0)
        {
            deadSynapses.push_back(s);
        }
    }
    //delete dead synapses
    //this will lead to a synapse count less than SAMPLE_SIZE so that new synapses can grow to previous winners
    if(deadSynapses.size() > 0)
    {
        //copy synapses excluding dead ones
        auto newIndices = new std::vector<unsigned short>;
        std::vector<unsigned char> newPerms;
        for(unsigned int s = 0; s < dend->potentialIndices->size(); s ++)
        {
            if(BitArray::in(s, &deadSynapses) < 0)
            {
                newIndices->push_back(dend->potentialIndices->at(s));
                newPerms.push_back(dend->permanences[s]);
            }
        }
        delete dend->potentialIndices;
        dend->potentialIndices = newIndices;
        dend->permanences = newPerms;
    }
    //grow new synapses to previous winner cells, up to SAMPLE_SIZE
    int newSynapses = SAMPLE_SIZE - dend->potentialIndices->size();
    auto candidates = prevWinners->onBits();
    //randomly sample the winner cells to save memory on large TMs
    //and to select specific cells from large unions to represent the previous pattern in the sequence
    while (newSynapses > 0 && candidates.size() > 0) {
        //choose a random winner cell
        unsigned short ran = rand() % candidates.size();
        unsigned short preSynaptic = candidates[ran];
        //remove it from the list of candidates
        candidates.erase(candidates.begin() + ran);
        //if it's not already connected, grow a new synapse
        if(BitArray::in(preSynaptic, dend->potentialIndices) < 0)
        {
            dend->potentialIndices->push_back(preSynaptic);
            dend->permanences.push_back(INITIAL_PERMANENCE);
            newSynapses --;
        }
    }
}

void TemporalMemory::fitTM(BitArray* distalActive, BitArray* distalWinners)
{
    //for each column
    BitArray* activeCols = sp_->activity();
    for(unsigned int col = 0; col < sp_->size(); col ++)
    {
        //if the column is active
        if(activeCols->at(col))
        {
            //check if
            bool bursting = true;
            for(unsigned int cell = 0; cell < height_; cell ++)
            {
                unsigned int cellIndex = col * height_ + cell;
                if(predicted_->at(cellIndex))
                {
                    bursting = false;
                    winners_->set(cellIndex, true);

                    //learn on active dendrites
                    auto distal = neurons[cellIndex]->distal;
                    for(unsigned int d = 0; d < distal->size(); d ++)
                    {
                        //for each active dendrite
                        Dendrite* dend = distal->at(d);
                        if(dend->active_)
                        {
                            fitDistalDendrite(dend, distalActive, distalWinners);
                        }
                    }
                }
            }
            //else the column is bursting
            if(bursting)
            {
                //find the best matching segment to the previous active cells,
                //choose that segment for learning and choose it's cell as the winner.
                //also count segments in case there are no matches
                unsigned short bestMatch = 0;
                unsigned int winner = 0;
                unsigned int learningDendrite = 0;
                unsigned int minSegments = 1 << 20;
                for(unsigned int cell = 0; cell < height_; cell ++)
                {
                    unsigned int cellIndex = col * height_ + cell;
                    auto distal = neurons[cellIndex]->distal;

                    if(distal->size() < minSegments)
                    {
                        minSegments = distal->size();
                    }

                    for(unsigned int d = 0; d < distal->size(); d ++)
                    {
                        if(distal->at(d)->potentialOverlap > bestMatch &&
                                distal->at(d)->potentialOverlap > LEARNING_THRESHOLD)
                        {
                            bestMatch = distal->at(d)->potentialOverlap;
                            winner = cellIndex;
                            learningDendrite = d;
                        }
                    }
                }
                //if there were no matching segments then grow a new segment on the cell with the fewest segments
                if(bestMatch == 0)
                {
                    //there may be ties for fewest segments
                    std::vector<unsigned int> tiedCells;
                    for(unsigned int cell = 0; cell < height_; cell ++)
                    {
                        unsigned int cellIndex = col * height_ + cell;
                        auto distal = neurons[cellIndex]->distal;
                        if(distal->size() == minSegments)
                        {
                            tiedCells.push_back(cellIndex);
                        }
                    }
                    //choose a random cell from the ties
                    unsigned int ran = rand() % tiedCells.size();
                    winner = tiedCells[ran];
                    winners_->set(winner, true);

                    Dendrite* newDend = new Dendrite;
                    fitDistalDendrite(newDend, distalActive, distalWinners);

                    neurons[winner]->distal->push_back(newDend);


                }
                //if there were matching segments, learn on the best matching segment
                else
                {
                    //TODO there may be ties for best match
                    winners_->set(winner, true);
                    Dendrite* dend = neurons[winner]->distal->at(learningDendrite);

                    fitDistalDendrite(dend, distalActive, distalWinners);

                }

            }
        }
        //the column is inactive
        else {
            //prediced cells in this column were incorrect
            for(unsigned int cell = 0; cell < height_; cell ++)
            {
                unsigned int cellIndex = col * height_ + cell;
                if(predicted_->at(cellIndex))
                {
                    //punish active dendrites
                    for(unsigned int d = 0; d < neurons[cellIndex]->distal->size(); d ++)
                    {
                        Dendrite* dend = neurons[cellIndex]->distal->at(d);
                        if(dend->overlap >= ACTIVE_THRESHOLD)
                        {
                            //punish active synapses
                            std::vector<unsigned short> deadSynapses;
                            for(unsigned int s = 0; s < dend->potentialIndices->size(); s ++)
                            {
                                unsigned int inputIndex = dend->potentialIndices->at(s);
                                if(distalActive->at(inputIndex))
                                {
                                    dend->permanences[s] = std::max(0, dend->permanences[s] - PREDICTED_DECREMENT);
                                    if(dend->permanences[s] == 0)
                                    {
                                        deadSynapses.push_back(s);
                                    }
                                }
                            }
                            //delete dead synapses
                            if(deadSynapses.size() > 0)
                            {
                                //copy synapses excluding dead ones
                                auto newIndices = new std::vector<unsigned short>;
                                std::vector<unsigned char> newPerms;
                                for(unsigned int s = 0; s < dend->potentialIndices->size(); s ++)
                                {
                                    if(BitArray::in(s, &deadSynapses) < 0)
                                    {
                                        newIndices->push_back(dend->potentialIndices->at(s));
                                        newPerms.push_back(dend->permanences[s]);
                                    }
                                }
                                delete dend->potentialIndices;
                                dend->potentialIndices = newIndices;
                                dend->permanences = newPerms;
                            }
                        }
                    }
                }
            }

        }
    }

}

void TemporalMemory::step()
{
    delete prevActive;
    delete prevPredicted;
    delete prevWinners;
    delete burstingColumns;

    prevActive = active_;
    prevPredicted = predicted_;
    prevWinners = winners_;

    active_ = new BitArray(static_cast<unsigned short>(neurons.size()), 0, SPARSE);
    predicted_ = new BitArray(static_cast<unsigned short>(neurons.size()), 0, SPARSE);
    winners_ = new BitArray(static_cast<unsigned short>(neurons.size()), 0, SPARSE);
    burstingColumns = new BitArray(sp_->size(), 0, SPARSE);
}

unsigned int TemporalMemory::countSynapses()
{
    unsigned int totalSyns = 0;
    for(unsigned int n = 0; n < neurons.size(); n ++)
    {
        for(unsigned int d = 0; d < neurons[n]->distal->size(); d ++)
        {
            Dendrite* dend = neurons[n]->distal->at(d);
            totalSyns += dend->potentialIndices->size();
        }
    }
    return totalSyns;
}

BitArray* TemporalMemory::active(int t)
{
    if(t == 0)
    {
        return active_;
    }
    else if (t == -1)
    {
        return prevActive;
    }
}

BitArray* TemporalMemory::prediced(int t)
{
    if(t == 0)
    {
        return predicted_;
    }
    else if (t == -1)
    {
        return  prevPredicted;
    }
}

BitArray* TemporalMemory::winners(int t)
{
    if(t == 0)
    {
        return winners_;
    }
    else if (t == -1)
    {
        return prevWinners;
    }
}

void TemporalMemory::print()
{
    std::cout << std::endl;
    for(unsigned int cell = 0; cell < height_; cell ++)
    {
        for(unsigned int col = 0; col < sp_->size(); col ++)
        {
            unsigned int index = col * height_ + cell;

            std::string symbol;
            if(winners_->at(index))
            {
                symbol = "[w]";
            }
            else if(active_->at(index))
            {
                symbol = "(*)";
            } else if (predicted_->at(index))
            {
                symbol = "( )";
            }
            else symbol = " . ";

            std::cout << symbol;

        }
        std::cout << std::endl;
    }
}
