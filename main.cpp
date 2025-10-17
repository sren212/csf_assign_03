#include <iostream>
#include <bits/stdc++.h>
#include "cache_fns.h"
using namespace std;

int main( int argc, char **argv ) {
  //all variables from argv
  int setNum;
  int blockNum;
  int bytesPerBlock;
  bool writeAllocate; 
  bool writeThrough; //true = write-through, false = write-back
  bool eviction; //true = lru, false = fifo

  if(argc != 7){
    cerr << "INCORRECT ARGUMENT INPUT" << std::endl;
    return 1;
  }

  //number of sets in cache
  try {
    setNum = std::stoi(argv[1]);

    if(!isPowerOfTwo(setNum)){
      throw std::invalid_argument("Number of sets must be a positive power-of-2");
    }
  }catch(const std::invalid_argument& e){
    cerr << "Invalid argument: " << e.what() << std::endl;
    return 1;
  }

  //number of blocks in each set
  try {
    blockNum = std::stoi(argv[2]);

    if(!isPowerOfTwo(blockNum)){
      throw std::invalid_argument("Number of blocks must be a positive power-of-2");
    }
  }catch(const std::invalid_argument& e){
    cerr << "Invalid argument: " << e.what() << std::endl;
    return 1;
  }

  //number of bytes in each block
  try {
    bytesPerBlock = std::stoi(argv[3]);

    if(!isPowerOfTwo(bytesPerBlock)){
      throw std::invalid_argument("Bytes per block must be a positive power-of-2");
    }else if(bytesPerBlock < 4){
      throw std::invalid_argument("Bytes per block must be greater than or equal to 4");
    }
  }catch(const std::invalid_argument& e){
    cerr << "Invalid argument: " << e.what() << std::endl;
    return 1;
  }

  //write-allocate
  try{
    if(strcmp(argv[4], "write-allocate")){
      writeAllocate = true;
    }else if(strcmp(argv[4], "no-write-allocate")){
      writeAllocate = false;
    }else{
      throw std::invalid_argument("must enter write-allocate/no-write-allocate for 4th argument");
    }
  }catch(const std::invalid_argument& e){
    cerr << "Invalid argument: " << e.what() << std::endl;
    return 1;
  }

  //write-through / write-back
  try{
    if(strcmp(argv[5], "write-through")){
      writeThrough = true;
    }else if(strcmp(argv[5], "write-back")){
      if(!writeAllocate){
        throw std::invalid_argument("cannot specify write-back and no-write-allocate at the same time");
      }
      writeThrough = false;
    }else{
      throw std::invalid_argument("must enter write-through/no-write-back for 5th argument");
    }
  }catch(const std::invalid_argument& e){
    cerr << "Invalid argument: " << e.what() << std::endl;
    return 1;
  }

  //lru / fifo
  try{
    if(strcmp(argv[6], "lru")){
      eviction = true;
    }else if(strcmp(argv[6], "fifo")){
      eviction = false;
    }else{
      throw std::invalid_argument("must enter lru / fifo for 6th argument");
    }
  }catch(const std::invalid_argument& e){
    cerr << "Invalid argument: " << e.what() << std::endl;
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

  /*

  // open file
  ifstream f(argv[7]);

  // check for errors opening the file
  if (!f.is_open()) {
    cerr << "ERROR OPENING TRACE FILE";
    return 1;
  }

  // read file and do operations
  string line;
  while (getline(f, line)) {

    stringstream line_stream(line);
    string field;

    // first field is load/store operation
    getline(line_stream, field, ' '); 
    string opString = field;
    
    // second field is memory address
    getline(line_stream, field, ' ');
    string addressString = field;

    // update total loads/stores:
    if (opString == "l"){
      loads++;
    } else {
      stores++;
    }

    // do operations
  }

  f.close();
  */

  std::string line;
  while(std::getline(std::cin, line)){
    //do operations on "line" for each line of input
  }

  // output summary info
  printSummary(loads, stores, loadHits, loadMisses, storeHits, storeMisses, cycles);



  return 0;
}
