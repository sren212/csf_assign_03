#include <iostream>
#include <bits/stdc++.h>
#include "cache_fns.h"
#include <sstream>
#include <string>
using namespace std;

int main( int argc, char **argv ) {
  //all variables from argv
  int setNum;
  int blockNum;
  int bytesPerBlock;
  bool writeAllocate; 
  bool writeThrough; //true = write-through, false = write-back
  bool lru; //true = lru, false = fifo

  if(argc != 7){
    cerr << "INCORRECT ARGUMENT INPUT" << endl;
    return 1;
  }

  //number of sets in cache
  try {
    setNum = stoi(argv[1]);

    if(!isPowerOfTwo(setNum)){
      throw invalid_argument("Number of sets must be a positive power-of-2");
    }
  }catch(const invalid_argument& e){
    cerr << "Invalid argument: " << e.what() << endl;
    return 1;
  }

  //number of blocks in each set
  try {
    blockNum = stoi(argv[2]);

    if(!isPowerOfTwo(blockNum)){
      throw invalid_argument("Number of blocks must be a positive power-of-2");
    }
  }catch(const invalid_argument& e){
    cerr << "Invalid argument: " << e.what() << endl;
    return 1;
  }

  //number of bytes in each block
  try {
    bytesPerBlock = stoi(argv[3]);

    if(!isPowerOfTwo(bytesPerBlock)){
      throw invalid_argument("Bytes per block must be a positive power-of-2");
    }else if(bytesPerBlock < 4){
      throw invalid_argument("Bytes per block must be greater than or equal to 4");
    }
  }catch(const invalid_argument& e){
    cerr << "Invalid argument: " << e.what() << endl;
    return 1;
  }

  //write-allocate
  try{
    if(strcmp(argv[4], "write-allocate") == 0){
      writeAllocate = true;
    }else if(strcmp(argv[4], "no-write-allocate") == 0){
      writeAllocate = false;
    }else{
      throw invalid_argument("must enter write-allocate/no-write-allocate for 4th argument");
    }
  }catch(const invalid_argument& e){
    cerr << "Invalid argument: " << e.what() << endl;
    return 1;
  }

  //write-through / write-back
  try{
    if(strcmp(argv[5], "write-through") == 0){
      writeThrough = true;
    }else if(strcmp(argv[5], "write-back") == 0){
      if(!writeAllocate){
        throw invalid_argument("cannot specify write-back and no-write-allocate at the same time");
      }
      writeThrough = false;
    }else{
      throw invalid_argument("must enter write-through/no-write-back for 5th argument");
    }
  }catch(const invalid_argument& e){
    cerr << "Invalid argument: " << e.what() << endl;
    return 1;
  }

  //lru / fifo
  try{
    if(strcmp(argv[6], "lru") == 0){
      lru = true;
    }else if(strcmp(argv[6], "fifo") == 0){
      lru = false;
    }else{
      throw invalid_argument("must enter lru / fifo for 6th argument");
    }
  }catch(const invalid_argument& e){
    cerr << "Invalid argument: " << e.what() << endl;
    return 1;
  }

  /* init vars representing total loads, stores, load/store hits, load/store misses, total cycles*/
  int loads = 0;
  int stores = 0;
  int loadHits = 0;
  int loadMisses = 0;
  int storeHits = 0;
  int storeMisses = 0;
  int cycles = 0;

  /* init cache */
  Cache cache;
  initCache(setNum, blockNum, &cache);

  string line;
  while(getline(cin, line)){
    stringstream line_stream(line);
    string field;

    // first field is load/store operation
    getline(line_stream, field, ' '); 
    string opString = field;
    
    // second field is memory address
    getline(line_stream, field, ' ');
    string addressString = field;

    // do operations
    //split hexadecimal from "0x"
    if(addressString.length() == 10){
      addressString.erase(0, 2);
    }else{
      cerr << "invalid address in trace file" << endl;
      return 2;
    }

    // convert hexadecimal into binary, then split into tag and index
    uint32_t tag;
    uint32_t index;
    divAddress(convertHexDec(addressString), bytesPerBlock, setNum, &tag, &index);
    
    //variable to account for whether we evicted a dirty slot during process
    bool evict_dirty = false;

    //search if cache contains data
    bool hit = isHit(&cache, tag, index);

    //update hit/miss & cache
    if(hit && opString == "l"){
      loads++;
      loadHits++;
      cycles++;
      evict_dirty = updateCacheLoad(&cache, tag, index, hit, lru);
    }else if(hit && opString == "s"){
      stores++;
      storeHits++;
      cycles++;
      evict_dirty = updateCacheStore(&cache, tag, index, writeAllocate, writeThrough, hit, lru);
    }else if(!hit && opString == "l"){
      loads++;
      loadMisses++;
      cycles += bytesPerBlock*100;
      evict_dirty = updateCacheLoad(&cache, tag, index, hit, lru);
    }else{
      stores++;
      storeMisses++;
      if (writeAllocate) { // on a store miss + no-write-allocate, we do not affect the cache
        cycles += bytesPerBlock*100;
      }
      evict_dirty = updateCacheStore(&cache, tag, index, writeAllocate, writeThrough, hit, lru);
    }

    // add to cycles if we evicted a dirty bit and we are using write-back
    if (evict_dirty && !writeThrough) {
      cycles += bytesPerBlock*100;
    }

    // add to cycles if we used write-through
    if (writeThrough) {
      cycles += bytesPerBlock*100 + 1;
    }
  }

  // output summary info
  printSummary(loads, stores, loadHits, loadMisses, storeHits, storeMisses, cycles);

  return 0;
}
