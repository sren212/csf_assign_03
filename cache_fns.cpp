#include <iostream>
#include "cache_fns.h"

bool isPowerOfTwo(int n){
    return n > 0 && (n & (n-1)) == 0;
}

//converts hexadecimal to binary
uint32_t convertHexDec(string hex){
    //TODO
}

//divides address into tag, index, offset
//modifies pointer of tag and index
//returns: void
void divAddress(uint32_t address, int *tag, int *index){
    //TODO
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