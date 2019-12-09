#include "bitarray.h"

BitArray::BitArray(unsigned int size, float expectedSparsity, performanceHint hint)
{
    size_ = size;
    hint_ = hint;
    expectedSparsity_ = expectedSparsity;

    unsigned int numberOfOnBits = static_cast<unsigned int>(size * expectedSparsity);
    //for speed, store the bits in an array for random access
    if (hint == DENSE)
    {
        bits_ = new unsigned char[size / 8 + 1];
        memset(bits_, 0, sizeof (bits_));
        onBits_ = nullptr;
    }
    //for memory, only store a list of on bits
    else if(hint == SPARSE)
    {
        bits_ = nullptr;
        onBits_ = new std::vector<unsigned short>;
    }
    //if no performance hint is given, use a heuristic to determine the best storage method
    else
    {
        //comparing bitArrays might take N^2 time (N = number of on bits) if the only the on bits are stored.
        //only store the on bits if it's not too slow
        if(numberOfOnBits*numberOfOnBits > size)
        {
            bits_ = new unsigned char[size / 8 + 1];
            memset(bits_, 0, sizeof (bits_));
            onBits_ = nullptr;
        }
        else
        {
            bits_ = nullptr;
            onBits_ = new std::vector<unsigned short>;
        }
    }
}

BitArray::BitArray(const BitArray& other)
{
    size_ = other.size_;
    if(other.bits_)
    {
        bits_ = new unsigned char[other.size_ / 8 + 1];
        memcpy(bits_, other.bits_, other.size_ / 8 + 1);
        onBits_ = nullptr;
    }
}

BitArray::~BitArray()
{

}

int in(unsigned int value, std::vector<unsigned short>* list)
{
    for(int i = 0; i < list->size(); i ++)
    {
        if(value == list->at(i))
        {
            return i;
        }
    }
    return -1;
}

void BitArray::set(unsigned int index, bool value)
{
    //if the bits are stored, use bitwise operations to set
    if(bits_)
    {
        int byte = index / 8;
        int bit = index % 8;
        unsigned char bitmatch = 1 << bit;
        if(value)
        {
            bits_[byte] |= bitmatch;
        }
        else
        {
            bits_[byte] &= ~bitmatch;
        }
    }
    else if(onBits_)
    {
        //find where the value 'index' is in the list of on bits
        int where = in(index, onBits_);
        //if it's not in the list yet
        if(where < 0)
        {
            //if the value is true, add the index to the list of on bits
            if(value)
            {
                onBits_->push_back(index);
            }
            //otherwise don't add it
        }
        //if it's already in the list
        else
        {
            //if the value is false, remove it from the list of on bits
            if(!value)
            {
                onBits_->erase(onBits_->begin() + where);
            }
            //otherwise don't re-add it
        }
    }
}

void BitArray::setRange(int start, int end, bool value)
{
    unsigned int range = end - start;
    if(bits_)
    {
        //if the range is long enough, we can save time by setting entire bytes at a time
        if(range > 16)
        {
            //the range probably won't fit evenly between bytes, there will be left and right tails.
            //set the left tail end using bitwise operations
            int next8th = (start / 8 + 1) * 8;
            for(int b = start; b < next8th; b ++)
            {
                set(b, value);
            }
            //set the middle bits by setting entire bytes at a time
            int numMiddleBytes = (range - (next8th - start)) / 8;
            int firstByte = next8th / 8;
            int lastByte = firstByte + numMiddleBytes;
            for(int by = firstByte; by < lastByte; by ++)
            {
                if(value)
                {
                    bits_[by] = 255;
                }
                else
                {
                    bits_[by] = 0;
                }
            }
            //set the right tail end using bitwise operations
            int rightBitStart = lastByte * 8;
            for(int b = rightBitStart; b <= end; b ++)
            {
                set(b, value);
            }
        }
        else
        {
            //if the interval is short, just use bitwise ops
            for(int b = start; b <= end; b ++)
            {
                set(b, value);
            }
        }
    }
    else if(onBits_)
    {
        for(int i = start; i <= end; i ++)
        {
            set(i, value);
        }
    }


}

bool BitArray::at(unsigned int index) const
{
    //if the bits are stored, use bitwise operations to get
    if(bits_)
    {
        int byte = index / 8;
        int bit = index % 8;
        unsigned char bitmatch = 1 << bit;

        bool ret = bits_[byte] & bitmatch;
        return ret;
    }
    else if(onBits_)
    {
        //return true if the index is in the list of on bits
        return in(index, onBits_) >= 0;
    }
}

//brackets return a proxy object so that it can use custom logic for setting
Proxy BitArray::operator[](unsigned int index)
{
    return Proxy(this, index);
}

//the proxy object has a custom assignment operator that assigns to the bitArray
Proxy& Proxy::operator=(bool value)
{
    array->set(index, value);
    return *this;
}

//the proxy has an implicit bool conversion for getting values
Proxy::operator bool()
{
    return array->at(index);
}

//unused?
bool BitArray::operator[](unsigned int index) const
{
    return at(index);
}

//subsets the array using a list of indices
BitArray BitArray::operator[](const std::vector<unsigned short>& indices)
{
    //add the bit at each index to the array
    BitArray ret(indices.size());
    for(int i = 0; i < indices.size(); i ++)
    {
        ret[i] = at(indices[i]);
    }
    return ret;
}

std::vector<unsigned short> BitArray::onBits()
{
    if(bits_)
    {
        //add each on bit to the list
        std::vector<unsigned short> on;
        for(unsigned short i = 0; i < size_; i ++)
        {
            if(at(i))
            {
                on.push_back(i);
            }
        }
        return on;
    }
    else if(onBits_)
    {
        return *onBits_;
    }
}

void BitArray::clear()
{
    if(bits_)
    {
        int bytes = size_  / 8 + 1;
        for(int b = 0; b < bytes; b ++)
        {
            bits_[b] = 0;
        }
    }
    else if(onBits_)
    {
        onBits_->clear();
    }

}

BitArray BitArray::operator|(const BitArray & arr)
{
    if(size_ != arr.size_)
    {
        std::cout << "arrays are not the same size" << std::endl;
    }
    performanceHint hint = NONE;
    float sparsity = 1.0f;
    if(hint_ == arr.hint_)
    {
        if(hint_ == SPARSE)
        {
            hint = SPARSE;
        }
        else if(hint_ == DENSE)
        {
            hint = DENSE;
        }
        else
        {
            sparsity = std::max(expectedSparsity_, arr.expectedSparsity_);
            hint = NONE;
        }
    }
    else
    {

    }

    BitArray ret(size_, sparsity, hint);
    if(ret.bits_)
    {
        int numBytes = size_ / 8 + 1;
        for(int b = 0; b < numBytes; b ++)
        {
            ret.bits_[b] = bits_[b] | arr.bits_[b];
        }
    }
    return ret;
}

//helper for printing
int countDigits(int num)
{
    int count = 0;
    do {
        num /= 10;
        count ++;
    } while(num != 0);
    return count;
}

void BitArray::print()
{
    int maxIndex = size_ - 25;
    int maxDigits = countDigits(std::max(0, maxIndex));

    for(int i = 0; i < size_; i ++)
    {

        bool every25th = i % 25 == 0;
        bool every5th = (i + 1) % 5 == 0;

        //print a new line and the index every 25 bits
        if(every25th)
        {
            std::cout << "\n[" << i;
            int digits = countDigits(i);
            int padd = maxDigits - digits;
            for(int p = 0; p < padd; p ++)
            {
                std::cout << " ";
            }
            std::cout << "] ";
        }

        std::cout << at(i) << " ";
        //print a space between every 5th bit for easy viewing
        if(every5th & !every25th)
        {
            std::cout << " ";
        }

    }

}

unsigned int BitArray::size()
{
    return size_;
}
