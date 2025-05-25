#pragma once
#include <cstdint>

#define CIRCULAR_SHIFT_LEFT_32_BITS(x, r) (((x) << (r)) | ((x) >> (32 - (r))))

namespace Brokkr
{
    class Hash
    {
    public:
        static uint32_t Murmur3Hash(const void* key, const size_t len, const uint32_t seed);
    private:
        static uint32_t Murmur3HashFinalizer(uint32_t hash);
    };
}

