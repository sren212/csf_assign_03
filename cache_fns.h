#ifndef CACHE_FUNS_H
#define CACHE_FUNS_H

struct cacheVars{
    int setNum;
    int blockNum;
    int bytesPerBlock;
    bool writeAllocate; 
    bool writeThrough; //true = write-through, false = write-back
    bool eviction; //true = lru, false = fifo
};

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