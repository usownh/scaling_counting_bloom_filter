#include "scalingbloomfilter.h"
#define ERROR_TIGHTENING_RATIO 0.5

ScalingBloomFilter::ScalingBloomFilter(unsigned int capacity, double error_rate)
{
    CountingBloomFilter *cur_bloom;

    scaling_bloom_t *bloom;



    this->capacity = capacity;
    this->error_rate = error_rate;
    this->num_blooms = 0;

    if (!(cur_bloom = new_counting_bloom_from_scale(bloom))) {
        fprintf(stderr, "Error, Could not create counting bloom\n");
        free_scaling_bloom(bloom);
        return NULL;
    }
    cur_bloom->count = 0;
    cur_bloom->header->id = 0;

    this->header->mem_seqnum = 1;
}

ScalingBloomFilter::~ScalingBloomFilter()
{
}


bool ScalingBloomFilter::Add(const char *s, size_t len, uint64_t id)
{
    int i;
    uint64_t seqnum;

    CountingBloomFilter *cur_bloom = NULL;
    for (i = this->num_blooms - 1; i >= 0; i--) {
        cur_bloom = this->blooms[i];
        if (id >= cur_bloom->header->id) {
            break;
        }
    }

    seqnum = scaling_bloom_clear_seqnums(bloom);

    if ((id > this->header->max_id) && (cur_bloom->count >= cur_bloom->capacity - 1)) {
        cur_bloom = new_counting_bloom_from_scale(bloom);
        cur_bloom->header->count = 0;
        cur_bloom->header->id = this->header->max_id + 1;
    }
    if (this->header->max_id < id) {
        this->header->max_id = id;
    }
    cur_bloom->Add( s, len);

    this->header->mem_seqnum = seqnum + 1;

    return true;
}

bool ScalingBloomFilter::Remove(const char *s, size_t len, uint64_t id)
{
    CountingBloomFilter *cur_bloom;
    int i;
    uint64_t seqnum;

    for (i = this->num_blooms - 1; i >= 0; i--) {
        cur_bloom = this->blooms[i];
        if (id >= cur_bloom->id) {
            seqnum = scaling_bloom_clear_seqnums(bloom);

            cur_bloom->Remove( s, len);

            this->header->mem_seqnum = seqnum + 1;
            return true;
        }
    }
    return false;
}

bool ScalingBloomFilter::Check(const char *s, size_t len)
{
    int i;
    CountingBloomFilter *cur_bloom;
    for (i = this->num_blooms - 1; i >= 0; i--) {
        cur_bloom = bloom->blooms[i];
        if (cur_bloom->Check(s, len)) {
            return true;
        }
    }
    return false;
}

CountingBloomFilter ScalingBloomFilter::ToCountingBloomFromScale()
{
    int i;
    double error_rate;
    error_rate = this->error_rate * (pow(ERROR_TIGHTENING_RATIO, this->num_blooms + 1));

    CountingBloomFilter cur_bloom(this->capacity,error_rate);

    this->blooms.push_back(cur_bloom);

    this->bitmap.Resize(this->num_bytes + cur_bloom.num_bytes);

    this->num_blooms++;
    for (i = 0; i < this->num_blooms; i++) {
        offset = bloom->blooms[i]->offset - sizeof(counting_bloom_header_t);
        bloom->blooms[i]->header = (counting_bloom_header_t *) (bloom->bitmap->array + offset);
    }

    this->num_bytes += cur_bloom.num_bytes;
    cur_bloom.bitmap = this->bitmap;

    return cur_bloom;
}


uint64_t ScalingBloomFilter::scaling_bloom_clear_seqnums(scaling_bloom_t *bloom)
{
    uint64_t seqnum;

    if (bloom->header->disk_seqnum != 0) {
        // disk_seqnum cleared on disk before any other changes
        bloom->header->disk_seqnum = 0;
        bitmap_flush(bloom->bitmap);
    }
    seqnum = bloom->header->mem_seqnum;
    bloom->header->mem_seqnum = 0;
    return seqnum;
}

int ScalingBloomFilter::scaling_bloom_flush(scaling_bloom_t *bloom)
{
    if (bitmap_flush(bloom->bitmap) != 0) {
        return -1;
    }
    // all changes written to disk before disk_seqnum set
    if (bloom->header->disk_seqnum == 0) {
        bloom->header->disk_seqnum = bloom->header->mem_seqnum;
        return bitmap_flush(bloom->bitmap);
    }
    return 0;
}

uint64_t ScalingBloomFilter::scaling_bloom_mem_seqnum(scaling_bloom_t *bloom)
{
    return bloom->header->mem_seqnum;
}

uint64_t ScalingBloomFilter::scaling_bloom_disk_seqnum(scaling_bloom_t *bloom)
{
    return bloom->header->disk_seqnum;
}
