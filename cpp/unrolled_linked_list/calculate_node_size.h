#ifndef __CALCULATE_NODE_SIZE__
#define __CALCULATE_NODE_SIZE__

#include <cstddef>
#include <cmath>

template <typename T>
size_t calculate_node_size(size_t count)
{
    if (0 == count)
        return 0;

    const char min_cache_size {64};

    // amount of memory for all data
    size_t memory_capacity {count * sizeof(T)};
    size_t cache_segs {0};

    cache_segs = ceil(static_cast<double>(memory_capacity) / min_cache_size) + 1;

    return cache_segs;
}

#endif
