#ifndef SCALINGBLOOMFILTER_H
#define SCALINGBLOOMFILTER_H
#include <countingbloomfilter.h>

typedef struct {
    uint64_t max_id;
    uint64_t mem_seqnum;
    uint64_t disk_seqnum;
} scaling_bloom_header_t;


class ScalingBloomFilter
{
public:
    ScalingBloomFilter(unsigned int capacity, double error_rate);
    ~ScalingBloomFilter();
    CountingBloomFilter ToCountingBloomFromScale();
    bool Check(const char *s, size_t len);
    bool Add(const char *s, size_t len, uint64_t id);
    bool Remove(const char *s, size_t len, uint64_t id);

    scaling_bloom_t *new_scaling_bloom(unsigned int capacity, double error_rate, const char *filename);
    int free_scaling_bloom(scaling_bloom_t *bloom);
    int scaling_bloom_flush(scaling_bloom_t *bloom);
    uint64_t scaling_bloom_mem_seqnum(scaling_bloom_t *bloom);
    uint64_t scaling_bloom_disk_seqnum(scaling_bloom_t *bloom);

    scaling_bloom_header_t *header;
    unsigned int capacity;
    unsigned int num_blooms;
    size_t num_bytes;
    double error_rate;
    vector<CountingBloomFilter> blooms;
    BitMap bitmap;
};

#endif // SCALINGBLOOMFILTER_H
