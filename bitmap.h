#ifndef BITMAP_H
#define BITMAP_H
#include <string>
#include <vector>
#include <iostream>
#include <stdint.h>
using std::string;
using std::vector;
using std::cerr;
using std::endl;
class BitMap
{
public:
    BitMap();
    BitMap(size_t bytes);
    void Resize(size_t newSize);
    void Increase(unsigned int index);
    void Decrease(unsigned int index);
    bool Check(unsigned int index);
    ~BitMap();
private:
    size_t bytes;
    vector<char>  array;
};

#endif // BITMAP_H
