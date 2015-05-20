#ifndef COUNTINGBLOOMFILTER_H
#define COUNTINGBLOOMFILTER_H
#include <cmath>
#include <bitmap.h>

class CountingBloomFilter
{
public:
    CountingBloomFilter(unsigned int capacity, double error_rate);
    ~CountingBloomFilter();

    void Add(const char *s, size_t len);
    void Remove(const char *s, size_t len);
    bool Check(const char *s, size_t len);
    void Hash(const char *key, size_t key_len);


    unsigned int capacity;
    unsigned int counts_per_func;
    vector<uint32_t> hashes;
    size_t nfuncs;
    size_t size;
    size_t num_bytes;
    double error_rate;
    BitMap bitmap;

    uint32_t count;
};

#endif // COUNTINGBLOOMFILTER_H
