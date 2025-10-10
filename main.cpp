#include <iostream>
#include <bits/stdc++.h>
#include "cache_fns.h"
using namespace std;

int main( int argc, char **argv ) {
  if(argc != 9){
    cerr << "INCORRECT ARGUMENT INPUT";
    return 1;
  }

  cacheVars vars;

  try {
    vars.setNum = std::stoi(argv[1]);
  }catch(const std::invalid_argument&){
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

  // output summary info
  printSummary(loads, stores, loadHits, loadMisses, storeHits, storeMisses, cycles);

  return 0;
}
