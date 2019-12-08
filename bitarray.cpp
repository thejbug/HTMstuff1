#include "bitarray.h"

BitArray::BitArray(unsigned int size, float sparsity, performanceHint hint)
{
    size_ = size;

    unsigned int numberOfOnBits = static_cast<unsigned int>(size * sparsity);
    if (hint == TIME)
    {

    }
    else if(hint == SPACE)
    {

    }
    else
    {
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

void BitArray::set(unsigned int index, bool value)
{
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

    }
}

bool BitArray::at(unsigned int index) const
{
    if(bits_)
    {
        int byte = index / 8;
        int bit = index % 8;
        unsigned char bitmatch = 1 << bit;

        return static_cast<bool>(bits_[byte] & bitmatch);
    }
}

Proxy& Proxy::operator=(bool value)
{
    array->set(index, value);
    return *this;
}

Proxy::operator bool()
{
    return array->at(index);
}

bool BitArray::operator[](unsigned int index) const
{
    return at(index);
}

Proxy BitArray::operator[](unsigned int index)
{
    return Proxy(this, index);
}

void BitArray::clear()
{
    int bytes = size_  / 8 + 1;
    for(int b = 0; b < bytes; b ++)
    {
        bits_[b] = 0;
    }
}

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
    int maxDigits = countDigits(size_ - 1);
    if(bits_)
    {
        for(int i = 0; i < size_; i ++)
        {
            int byte = i / 8;
            int bit = i % 8;
            unsigned char bitmatch = 1 << bit;
            bool every25th = i % 25 == 0;
            bool every5th = (i + 1) % 5 == 0;

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

            std::cout << static_cast<bool>(bits_[byte] & bitmatch) << " ";
            if(every5th & !every25th)
            {
                std::cout << " ";
            }

        }
    }

}

unsigned int BitArray::size()
{
    return size_;
}
