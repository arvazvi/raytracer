#pragma once

#ifndef RANDOM_H
#define RANDOM_H

#include <cstdint>

// Per-thread RNG state (xorshift64*). Each thread must have its own state so
// random_double() is safe to call concurrently and renders are deterministic.
inline uint64_t& random_state() {
    thread_local uint64_t state = 0x853c49e6748fea9bULL;
    return state;
}

inline void seed_random(uint64_t seed) {
    auto s = 0x9E3779B97F4A7C15ULL + seed;
    uint64_t z = s + 0x9E3779B97F4A7C15ULL;
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    z ^= z >> 31;
    random_state() = z ? z : 1;
}

// Returns a uniform double in [0, 1).
inline double random_double() {
    uint64_t& s = random_state();
    s ^= s >> 12;
    s ^= s << 25;
    s ^= s >> 27;
    return double((s * 0x2545F4914F6CDD1DULL) >> 11) * 0x1.0p-53;
}

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

#endif
