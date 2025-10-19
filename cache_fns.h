#ifndef CACHE_FUNS_H
#define CACHE_FUNS_H

#include <bits/stdc++.h>
#include <cstdint>
using namespace std;

//a "block" with extra attributes
struct Slot{
    uint32_t tag;
    bool valid, dirty;
    uint32_t load_ts, //load timestamp (calculate # of loads & stores that has happened thus far OR cycle count)
        access_ts;
};

struct Set{
    std::vector<Slot> slots;
};

struct Cache {
    std::vector<Set> sets;

    // init a cache and the sets inside of it to be a certain size
    Cache(int num_sets, int num_slots) {
        sets.resize(num_sets);
        for (size_t i = 0; i < sets.size(); i++) {
            sets[i].slots.resize(num_slots);
        }
    }
};

std::map<uint32_t, Slot *> index;

//checks if the number if a power of 2
bool isPowerOfTwo(int n);

// Print out a summary of the cache simulator.
//
// Parameters:
// loads - number of loads
// stores - number of stores
// loadHits - number of load hits
// loadMisses - number of load misses
// storeHits - number of store hits
// storeMisses - number of store misses
// cycles - number of cycles
void printSummary(int loads, int stores, int loadHits, int loadMisses, 
                  int storeHits, int storeMisses, int cycles);

#endif