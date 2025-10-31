Suwen Ren
- isHit, updateCacheLoad, updateAccessTS, updateLoadTS, 
  chooseEvict, updateSlot, markDirty, printSummary

Lena Shea
- initCache, isPowerOfTwo, convertHexDec, divAddress, 
  updateCacheStore, input handling + structure of main.cpp

Best cache report:

To determine the best cache, we ran several experiments using 
the input gcc.trace with a cache size of 2^12 bytes. We chose 
gcc.trace because it is a large dataset with a random 
distribution of loads and stores, making for a realistic test. 
We examined the effectiveness of the following parameters:

- fully associative vs. set associative vs. direct mapping
    - We determined that fully-associative caches performed in 
      the fewest cycles, followed in efficiency by 
      set-associative caches and direct-mapping caches. The 
      configuration 
      ./csim 1 1024 4 write-allocate write-back lru < gcc.trace
      performed the best.
- for a set-associative cache, the best number of sets
    - We found that the fewer the sets, the fewer the cycles 
      needed. The configuration 
      ./csim 2 512 4 write-allocate write-back lru < gcc.trace 
      performed best out of the set-associative caches.
- block size
    - We found that a small block size (e.g. 4 bytes) resulted 
      in the fewest cycles.
- fifo vs. lru
    - While the choice of eviction policy did not lead to 
      significant differences when other factors remained 
      constant, caches that employed the Least Recently 
      Used (LRU) policy consistently outperformed those using 
      the First In First Out (FIFO) method.
- write-allocate + write-back vs. no-write-allocate + write-through 
  vs. write-allocate + write-through
    - The combination of write-allocate + write-back performed.
      most efficiently, following with no-write-allocate + 
      write-through, and the least efficient combination was 
      write-allocate + write-through

From our tests, we conclude that the best configuration is a fully 
associative cache with the smallest block size possible, using a 
write-allocate + write-back policy and least-recently used. For a 
set-associative cache, the best configuration is the smallest number 
of sets possible (2 bytes) with the smallest block size possible, 
using write-allocate + write-back policy and least-recently used.
