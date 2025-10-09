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

#endif