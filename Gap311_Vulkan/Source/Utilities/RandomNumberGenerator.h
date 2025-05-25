#pragma once
#include <cstdint>

// Source: Took this from a GAP295 assignment as a temp I want to make a mersenne twister engine or methods eventually
class RandomNumberGenerator
{
    uint64_t m_state[2];

public:
    RandomNumberGenerator() : m_state{ 0, 0 } { }

    void SeedFromTime();

    uint64_t Rand();
    float FRand();  // returns a random number from 0 - 1, inclusive
    float SignedFRand();  // returns a random number from -1 to 1, inclusive
    uint64_t RandomRange(uint64_t min, uint64_t max);
};

