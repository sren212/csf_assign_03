#include <iostream>
#include <string>
#include <cmath>
#include "cache_fns.h"

bool isPowerOfTwo(int n){
    return n > 0 && (n & (n-1)) == 0;
}

//converts hexadecimal to int
uint32_t convertHexDec(string hex){
    return std::stoi(hex, nullptr, 16);
}

//divides address into tag, index, offset
//modifies pointer of tag and index
//returns: void
void divAddress(uint32_t address, int bytesPerBlock, int setNum, int *tag, int *index){
    //gets rid of offset
    address >> bytesPerBlock;

    //take index and store
    *index = address & static_cast<uint32_t>(std::log2(setNum));
    address >> (int)log2(setNum)+1;

    //take tag and store
    *tag = address;
}

//searches cache for where data is located/should be located
Slot searchCache(uint32_t tag, uint32_t index){
    //TODO
}

void printSummary(int loads, int stores, int loadHits, int loadMisses, 
                  int storeHits, int storeMisses, int cycles) {
    cout << "Total loads: " << loads << endl;
    cout << "Total stores: " << stores << endl;
    cout << "Load hits: " << loadHits << endl;
    cout << "Load misses: " << loadMisses << endl;
    cout << "Store hits: " << storeHits << endl;
    cout << "Store misses: " << storeMisses << endl;
    cout << "Total cycles: " << cycles << endl;
}