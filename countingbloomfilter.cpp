#include "countingbloomfilter.h"
#define SALT_CONSTANT 0x97c29b3a
extern "C"
{
#include <murmur.h>
}

CountingBloomFilter::CountingBloomFilter(unsigned int capacity, double error_rate)
{
    this->capacity = capacity;
    this->error_rate = error_rate;
    this->nfuncs = (int) ceil(log(1 / error_rate) / log(2));
    this->counts_per_func = (int) ceil(capacity * fabs(log(error_rate)) / (this->nfuncs * pow(log(2), 2)));
    this->size = this->nfuncs * this->counts_per_func;
    /* rounding-up integer divide by 2 of bloom->size */
    this->num_bytes = ((this->size + 1) / 2);
    this->hashes.resize(this->nfuncs);
    this->bitmap.Resize(this->num_bytes);
}

CountingBloomFilter::~CountingBloomFilter()
{
}

void CountingBloomFilter::Hash(const char *key, size_t key_len)
{
    unsigned int i;
    uint32_t checksum[4];

    MurmurHash3_x64_128(key, key_len, SALT_CONSTANT, checksum);
    uint32_t h1 = checksum[0];
    uint32_t h2 = checksum[1];

    for (i = 0; i < this->nfuncs; i++) {
        hashes[i] = (h1 + i * h2) % this->counts_per_func;
    }
}




void CountingBloomFilter::Add(const char *s, size_t len)
{
    unsigned int index, i, offset;
    this->Hash(s, len);

    for (i = 0; i < this->nfuncs; i++)
    {
        offset = i * this->counts_per_func;
        index = hashes[i] + offset;
        this->bitmap.Increase(index);
    }
    this->count++;
}

void CountingBloomFilter::Remove(const char *s, size_t len)
{
    unsigned int index, i, offset;

    this->Hash(s, len);

    for (i = 0; i < this->nfuncs; i++)
    {
        offset = i * this->counts_per_func;
        index = hashes[i] + offset;
        this->bitmap.Decrease(index);
    }
    this->count--;
}

bool CountingBloomFilter::Check(const char *s, size_t len)
{
    unsigned int index, i, offset;

    this->Hash(s, len);

    for (i = 0; i < this->nfuncs; i++)
    {
        offset = i * this->counts_per_func;
        index = hashes[i] + offset;
        if (!this->bitmap.Check(index)) return false;
    }
    return true;
}
