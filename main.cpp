#include <iostream>
#include <bits/stdc++.h>
#include "cache_fns.h"
using namespace std;

int main( int argc, char **argv ) {
  if(argc != 9){
    cerr << "INCORRECT ARGUMENT INPUT" << std::endl;
    return 1;
  }

  cacheVars vars;

  //number of sets in cache
  try {
    vars.setNum = std::stoi(argv[1]);

    if(!isPowerOfTwo(vars.setNum)){
      throw std::invalid_argument("Number of sets must be a positive power-of-2");
    }
  }catch(const std::invalid_argument&){
    cerr << "Invalid argument" << std::endl;
    return 1;
  }

  //number of blocks in each set
  try {
    vars.blockNum = std::stoi(argv[2]);

    if(!isPowerOfTwo(vars.blockNum)){
      throw std::invalid_argument("Number of blocks must be a positive power-of-2");
    }
  }catch(const std::invalid_argument&){
    cerr << "Invalid argument" << std::endl;
    return 1;
  }

  //number of bytes in each block
  try {
    vars.bytesPerBlock = std::stoi(argv[3]);

    if(!isPowerOfTwo(vars.bytesPerBlock)){
      throw std::invalid_argument("Bytes per block must be a positive power-of-2");
    }
  }catch(const std::invalid_argument&){
    cerr << "Invalid argument" << std::endl;
    return 1;
  }


  /* init vars representing total loads, stores, load/store hits, load/store misses, total cycles*/
  int loads = 0;
  int stores = 0;
  int load_hits = 0;
  int load_misses = 0;
  int store_hits = 0;
  int store_misses = 0;
  int cycles = 0;

  // open file
  ifstream f(argv[8]);

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
    string op_string = field;
    
    // second field is memory address
    getline(line_stream, field, ' ');
    string address_string = field;

    // update total loads/stores:
    if (op_string == "l"){
      loads++;
    } else {
      stores++;
    }

    // do operations
  }

  f.close();

  // output summary info
  cout << "Total loads: " << loads << endl;
  cout << "Total stores: " << stores << endl;
  cout << "Load hits: " << load_hits << endl;
  cout << "Load misses: " << load_misses << endl;
  cout << "Store hits: " << store_hits << endl;
  cout << "Store misses: " << store_misses << endl;
  cout << "Total cycles: " << cycles << endl;

  return 0;
}
