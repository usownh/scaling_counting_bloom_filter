#include "bitmap.h"

BitMap::BitMap()
{    
    this->bytes = 0;
}

BitMap::BitMap(size_t bytes)
{
    this->bytes = bytes;
    this->Resize(bytes);
}

void BitMap::Resize(size_t newSize)
{
    if(newSize<this->bytes)
    {
        cerr<<"bitmap resize error!"<<endl;
        return;
    }
    this->array.resize(newSize);
    this->bytes = newSize;
}


void BitMap::Increase(unsigned int index)
{
    unsigned long access = index / 2 ;
    if(access>=array.size())
    {
        cerr<<"Error!Bitmap increase index overflow!"<<endl;
        return;
    }
    uint8_t temp;
    uint8_t n = array[access];
    if (index % 2 != 0)
    {
        temp = (n & 0x0f);
        n = (n & 0xf0) + ((n & 0x0f) + 0x01);
    }
    else
    {
        temp = (n & 0xf0) >> 4;
        n = (n & 0x0f) + ((n & 0xf0) + 0x10);
    }

    if (temp == 0x0f)
    {
        cerr<<"Error, 4 bit int Overflow\n"<<endl;
        return;
    }
    array[access] = n;
}

void BitMap::Decrease(unsigned int index)
{
    unsigned long access = index / 2 ;
    if(access>=array.size())
    {
        cerr<<"Error!Bitmap decrease index overflow!"<<endl;
        return;
    }
    uint8_t temp;
    uint8_t n = array[access];

    if (index % 2 != 0)
    {
        temp = (n & 0x0f);
        n = (n & 0xf0) + ((n & 0x0f) - 0x01);
    }
    else
    {
        temp = (n & 0xf0) >> 4;
        n = (n & 0x0f) + ((n & 0xf0) - 0x10);
    }
    if (temp == 0x00)
    {
        cerr<<"Error, Decrementing zero\n"<<endl;
        return;
    }
    array[access] = n;
}

bool BitMap::Check(unsigned int index)
{
    unsigned long access = index / 2 ;
    if(access>=array.size())
    {
        cerr<<"Error!Bitmap check index overflow!"<<endl;
        return false;
    }
    if (index % 2 != 0 )
    {
        return array[access] & 0x0f;
    }
    else
    {
        return array[access] & 0xf0;
    }
}

BitMap::~BitMap()
{
}

