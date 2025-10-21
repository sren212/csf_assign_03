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

// Update the cache to represent its state after a load or store
void updateCache(Cache *cache, uint32_t tag, uint32_t index, char op_type, bool write_allocate, bool write_through, bool hit, bool lru){
    // load
    if (op_type == 'l') {
        updateCacheLoad(cache, tag, index, hit, lru);
    }
    // store
    else if (op_type == 's') {
        updateCacheStore(cache, tag, index, write_allocate, write_through, hit, lru);
    }
}

// Update the cache to represent its state after a load
void updateCacheLoad(Cache *cache, uint32_t tag, uint32_t index, bool hit, bool lru) {
    if (hit) {
        // find the set
        Set target_set = (*cache).sets[index];

        // find the slot
        Slot target_slot = target_set.slots[0];

        int index = 0;
        while(target_slot.tag != tag) {
            target_slot = target_set.slots[index++];
        }

        // update access_ts of the slot
        int curr_access = target_slot.access_ts;
        // if access_ts isn't already the max (slots.size() - 1), update slots
        if (curr_access != 0) {
            target_slot.access_ts = 0;
            
            // update access_ts of all other slots
            for (int i = 0; i < target_set.slots.size() - 1; i++) {
                // skip index of the slot we already updated
                if (i == index)
                    break;

                Slot curr_slot = target_set.slots[i];

                // skip invalid slots
                if (!curr_slot.valid)
                    break;
                
                // inrease the timestamp if it is less than the original access_ts of our target
                // otherwise, the timestamp will remain the same
                if (curr_slot.access_ts < curr_access) {
                    curr_slot.access_ts += 1;
                }
            }
        }
    }
}

// Update the cache to represent its state after a store
void updateCacheStore(Cache *cache, uint32_t tag, uint32_t index, bool write_allocate, bool write_through, bool hit, bool lru) {

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