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
bool updateCacheLoad(Cache *cache, uint32_t tag, uint32_t index, bool hit, bool lru) {
    bool evict_dirty = false;
    if (!hit) {
        bool found_slot = false;

        // see if there is an empty slot in cache[index]
        Set target_set = (*cache).sets[index];
        for (int i = 0; i < target_set.slots.size(); i++) {
            Slot curr = target_set.slots[i]; 
            // found an empty slot!
            if (!curr.valid) {
                // update slot
                curr.tag = tag;
                curr.valid = true;
                curr.dirty = false;
                found_slot = true;
                break;
            }
        }
        
        if (!found_slot) {
            // choose which slot to evict & update evict_dirty
            uint32_t evict_index = chooseEvict(cache, index, lru);
            Slot evict = (*cache).sets[index].slots[evict_index];
            evict_dirty = evict.dirty;
            
            // update the slot info
            evict.tag = tag;
            evict.valid = true;
            evict.dirty = false;
        }
    }

    // update timestamps
    updateAccessTS(cache, tag, index);
    updateLoadTS(cache, tag, index);
    return evict_dirty;
}

// Update the cache to represent its state after a store
bool updateCacheStore(Cache *cache, uint32_t tag, uint32_t index, bool write_allocate, bool write_through, bool hit, bool lru) {

}

// Update the cache to represent its state after a load
void updateAccessTS(Cache *cache, uint32_t tag, uint32_t index) {
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
                continue;

            Slot curr_slot = target_set.slots[i];

            // skip invalid slots
            if (!curr_slot.valid)
                continue;
            
            // inrease the timestamp if it is less than the original access_ts of our target
            // otherwise, the timestamp will remain the same
            if (curr_slot.access_ts < curr_access) {
                curr_slot.access_ts += 1;
            }
        }
    }
}

// Update the cache to represent its state after a load
void updateLoadTS(Cache *cache, uint32_t tag, uint32_t index) {
    Set target_set = (*cache).sets[index];
    // find the slot
    Slot target_slot = target_set.slots[0];

    int index = 0;
    while(target_slot.tag != tag) {
        target_slot = target_set.slots[index++];
    }

    // update load_ts of the slot
    int curr_access = target_slot.load_ts;
    // if load_ts isn't already the max (slots.size() - 1), update slots
    if (curr_access != 0) {
        target_slot.load_ts = 0;
        
        // update load_ts of all other slots
        for (int i = 0; i < target_set.slots.size() - 1; i++) {
            // skip index of the slot we already updated
            if (i == index)
                continue;

            Slot curr_slot = target_set.slots[i];

            // skip invalid slots
            if (!curr_slot.valid)
                continue;
            
            // inrease the timestamp if it is less than the original load_ts of our target
            // otherwise, the timestamp will remain the same
            if (curr_slot.load_ts < curr_access) {
                curr_slot.load_ts += 1;
            }
        }
    }
}

// Choose the slot to be evicted in the full set at the index.
uint32_t chooseEvict(Cache *cache, uint32_t index, bool lru) {
    Set target_set = (*cache).sets[index];
    uint32_t tag;

    // using least recently used
    if (lru) {
        tag = -1;
        uint32_t max_access_ts = -1;

        for(int i = 0; i < target_set.slots.size(); i++) {
            Slot curr_slot = target_set.slots[i];
            if (curr_slot.access_ts > max_access_ts) {
                max_access_ts = curr_slot.access_ts;
                tag = curr_slot.tag;
            }
        }

    } else { // using fifo
        tag = -1;
        uint32_t max_load_ts = -1;

        for(int i = 0; i < target_set.slots.size(); i++) {
            Slot curr_slot = target_set.slots[i];
            if (curr_slot.load_ts > max_load_ts) {
                max_load_ts = curr_slot.load_ts;
                tag = curr_slot.tag;
            }
        }
    }

    return tag;
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