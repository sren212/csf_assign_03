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
            for (size_t j = 0; j < sets[i].slots.size(); j++) {
                Slot slot = sets[i].slots[j];
                slot.valid = false;
                slot.dirty = false;
            }
        }
    }
};

//only add index if necessary
// std::map<uint32_t, Slot *> index;

//checks if the number if a power of 2
bool isPowerOfTwo(int n);

//converts hexadecimal to int
uint32_t convertHexDec(string hex);

//divides address into tag, index, offset
//modifies pointer of tag and index
//parameters: address, pointer of tag, pointer of index
//returns: void
void divAddress(uint32_t address, int bytesPerBlock, int setNum, uint32_t *tag, uint32_t *index);

//searches if cache contains data, choooses the correct slot for new data if not found
//parameters: tag, index
//returns: slot (where data is contained / should be stored)
Slot searchCache(uint32_t tag, uint32_t index);

// Update the cache to represent its state after a load or store
//
// Parameters:
// cache - the cache
// tag - the tag of the element
// index - the index of the element
// op_type - operation type: 'l' if load and 's' if store
// write_allocate - true if we are using write allocate and false if we are using no-write-allocate
// write_through - true if we are using write_through and false if we are using write_back
// hit - true if this operation had a hit and false if this operation had a miss
// lru - true if we are using lru, and false if we are using fifo
void updateCache(Cache *cache, uint32_t tag, uint32_t index, char op_type, bool write_allocate, bool write_through, bool hit, bool lru);

// Update the cache to represent its state after a load
//
// Parameters:
// cache - the cache
// tag - the tag of the element
// index - the index of the element
// hit - true if this operation had a hit and false if this operation had a miss
// lru - true if we are using lru, and false if we are using fifo
// returns true if we evicted a dirty slot
bool updateCacheLoad(Cache *cache, uint32_t tag, uint32_t index, bool hit, bool lru);

// Update the cache to represent its state after a store
//
// Parameters:
// cache - the cache
// tag - the tag of the element
// index - the index of the element
// write_allocate - true if we are using write allocate and false if we are using no-write-allocate
// write_through - true if we are using write_through and false if we are using write_back
// hit - true if this operation had a hit and false if this operation had a miss
// lru - true if we are using lru, and false if we are using fifo
// returns true if we evicted a dirty slot
bool updateCacheStore(Cache *cache, uint32_t tag, uint32_t index, bool write_allocate, bool write_through, bool hit, bool lru);

// Update the cache to represent its state after a load
//
// Parameters:
// cache - the cache
// tag - the tag of the element
// index - the index of the element
void updateAccessTS(Cache *cache, uint32_t tag, uint32_t index);

// Update the cache to represent its state after a load
//
// Parameters:
// cache - the cache
// tag - the tag of the element
// index - the index of the element
void updateLoadTS(Cache *cache, uint32_t tag, uint32_t index);

// Choose the slot to be evicted in the full set at the index.
//
// Parameters:
// cache - the cache
// index - the index of the set
// lru - whether we are using lru (true) or fifo (false)
// returns: the tag of the slot to evict
uint32_t chooseEvict(Cache *cache, uint32_t index, bool lru);

// Find an empty slot or evict an empty slot in the set at index and update with tag
//
// Parameters:
// cache - the cache
// tag - the tag of the element
// index - the index of the element
// lru - whether we are using lru (true) or fifo (false)
bool updateSlot(Cache *cache, uint32_t tag, uint32_t index, bool lru);

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