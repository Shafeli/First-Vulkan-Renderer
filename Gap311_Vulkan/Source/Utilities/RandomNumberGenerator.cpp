#include "RandomNumberGenerator.h"
#include <cassert>
#include <ctime>
#include <limits>

void RandomNumberGenerator::SeedFromTime()
{
    const uint64_t currTime = std::time(nullptr);
    m_state[0] = currTime;
    m_state[1] = currTime;
}

uint64_t RandomNumberGenerator::Rand()
{
    // Implementation from here:
    // https://en.wikipedia.org/wiki/Xorshift#xorshift+

    uint64_t t = m_state[0];
    uint64_t const s = m_state[1];
    m_state[0] = s;
    t ^= t << 23;		// a
    t ^= t >> 18;		// b -- Again, the shifts and the multipliers are tunable
    t ^= s ^ (s >> 5);	// c
    m_state[1] = t;
    return t + s;
}

float RandomNumberGenerator::FRand()
{
    static constexpr auto kMaxIntAsFloat = static_cast<float>(std::numeric_limits<uint64_t>::max());
    return Rand() / kMaxIntAsFloat;
}

float RandomNumberGenerator::SignedFRand()
{
    static constexpr auto kMaxIntAsFloat = static_cast<float>(std::numeric_limits<uint64_t>::max());

    const uint64_t initialRoll = Rand();
    constexpr float halfRandMax = kMaxIntAsFloat / 2.f;

    const float result = (static_cast<float>(initialRoll) - halfRandMax) / halfRandMax;
    return result;
}

uint64_t RandomNumberGenerator::RandomRange(uint64_t min, uint64_t max)
{
    assert(max >= min);
    const uint64_t range = max - min;
    const uint64_t result = (Rand() % range) + min;
    return result;
}
