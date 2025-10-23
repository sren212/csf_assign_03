#include <iostream>
#include <string>
#include <cmath>
#include "cache_fns.h"

void initCache(int num_sets, int num_slots, Cache *cache){
    (*cache).sets.resize(static_cast<size_t>(num_sets));
    for (size_t i = 0; i < (*cache).sets.size(); i++) {
        (*cache).sets[i].slots.resize(static_cast<size_t>(num_slots));
        for (size_t j = 0; j < (*cache).sets[i].slots.size(); j++) {
            Slot *slot = &((*cache).sets[i].slots[j]);
            (*slot).tag = 0;
            (*slot).valid = false;
            (*slot).dirty = false;
            (*slot).load_ts = 0;
            (*slot).access_ts = 0;
        }
    }
}

bool isPowerOfTwo(int n){
    return n > 0 && (n & (n-1)) == 0;
}

//converts hexadecimal to int
uint32_t convertHexDec(string hex){
    return static_cast<uint32_t>(std::stoul(hex, nullptr, 16));
}

//divides address into tag, index, offset
//modifies pointer of tag and index
//returns: void
void divAddress(uint32_t address, int bytesPerBlock, int setNum, uint32_t *tag, uint32_t *index){
    uint32_t offsetBits = 0;
    uint32_t x = bytesPerBlock;
    while((x >>= 1) != 0) ++offsetBits;
    
    //gets rid of offset
    address >>= offsetBits;

    uint32_t indexBits = 0;
    x = setNum;
    while((x >>= 1) != 0) ++indexBits;

    //take index and store
    uint32_t indexMask = (indexBits == 32) ? 0xFFFFFFFFu : ((1u << indexBits)-1u);
    *index = address & indexMask;
    address >>= indexBits;

    //take tag and store
    *tag = address;
}

// given a tag and an index of an element, determine whether it is a hit or a miss
bool isHit(Cache *cache, uint32_t tag, uint32_t index){
    Set target_set = (*cache).sets[index];
    
    // search set for a slot whose tag matches our target tag
    for(size_t i = 0; i < target_set.slots.size(); i++) {
        if (target_set.slots[i].valid && target_set.slots[i].tag == tag) {
            return true;
        }
    }

    return false;
}

// Update the cache to represent its state after a load
bool updateCacheLoad(Cache *cache, uint32_t tag, uint32_t index, bool hit, bool lru) {
    bool evict_dirty = false;

    // if it's a miss, we need to find an empty slot/evict a slot and update it
    if (!hit) {
        evict_dirty = updateSlot(cache, tag, index, lru, false);
    }

    // update timestamps
    updateAccessTS(cache, tag, index);
    updateLoadTS(cache, tag, index);
    return evict_dirty;
}

// Update the cache to represent its state after a store
bool updateCacheStore(Cache *cache, uint32_t tag, uint32_t index, bool write_allocate, bool write_through, bool hit, bool lru) {
    bool evict = false;

    if(!hit){
        if(write_allocate){
            evict = updateSlot(cache, tag, index, lru, true);
        }
    }

    if(hit || write_allocate){
        updateAccessTS(cache, tag, index);
        updateLoadTS(cache, tag, index);
    }

    return evict;
}


// Update the cache to represent its state after an access
void updateAccessTS(Cache *cache, uint32_t tag, uint32_t index) {
    Set *target_set = &(*cache).sets[index];
    int target_index = -1;

    for (uint32_t i = 0; i < target_set->slots.size(); i++) {
        if (target_set->slots[i].valid && target_set->slots[i].tag == tag) {
            target_index = i;
            break;
        }
    }

    if (target_index == -1) {
        return;
    }

    // update access_ts of the slot
    uint32_t old_access = target_set->slots[target_index].access_ts;
    target_set->slots[target_index].access_ts = 0;
    // if access_ts isn't already the max (slots.size() - 1), update slots
    for (uint32_t i = 0; i < target_set->slots.size(); i++) {
        // skip target index
        if ((int)i == target_index) {
            continue;
        }

        Slot *curr = &target_set->slots[i];
        // skip invalid slots
        if (!curr->valid) {
            continue;
        }
        if (curr->access_ts < old_access) {
            curr->access_ts++;
        }
    }
}

// Update the cache to represent its state after a load
void updateLoadTS(Cache *cache, uint32_t tag, uint32_t index) {
    Set *target_set = &(*cache).sets[index];
    int target_index = -1;

    for (uint32_t i = 0; i < target_set->slots.size(); i++) {
        if (target_set->slots[i].valid && target_set->slots[i].tag == tag) {
            target_index = i;
            break;
        }
    }

    if (target_index == -1) {
        return;
    }

    // update load_ts of the slot
    uint32_t old_access = target_set->slots[target_index].load_ts;
    target_set->slots[target_index].load_ts = 0;
    // if load_ts isn't already the max (slots.size() - 1), update slots
    for (uint32_t i = 0; i < target_set->slots.size(); i++) {
        // skip target index
        if ((int)i == target_index) {
            continue;
        }

        Slot *curr = &target_set->slots[i];
        // skip invalid slots
        if (!curr->valid) {
            continue;
        }
        if (curr->load_ts < old_access) {
            curr->load_ts++;
        }
    }
}

// Choose the slot to be evicted in the full set at the index.
uint32_t chooseEvict(Cache *cache, uint32_t index, bool lru) {
    Set target_set = cache->sets[index];
    int ans_index = -1;

    // using least recently used
    if (lru) {
        int max_access_ts = -1;

        // go through slots and find the slot with the highest access_ts
        for(uint32_t i = 0; i < target_set.slots.size(); i++) {
            Slot curr_slot = target_set.slots[i];
            if ((int)curr_slot.access_ts > max_access_ts) {
                max_access_ts = curr_slot.access_ts;
                ans_index = i;
            }
        }

    } else { // using fifo
        int max_load_ts = -1;

        // go through slots and find the slot with the highest load_ts
        for(uint32_t i = 0; i < target_set.slots.size(); i++) {
            Slot curr_slot = target_set.slots[i];
            if ((int)curr_slot.load_ts > max_load_ts) {
                max_load_ts = curr_slot.load_ts;
                ans_index = i;
            }
        }
    }

    return ans_index;
}

// find an empty slot or evict an empty slot in the set at index and update with tag
bool updateSlot(Cache *cache, uint32_t tag, uint32_t index, bool lru, bool store) {
    bool evict_dirty = false;

    // see if there is an empty slot in cache[index]
    Set *target_set = &cache->sets[index];
    for (uint32_t i = 0; i < (*target_set).slots.size(); i++) {
        Slot *curr = &target_set->slots[i]; 
        // found an empty slot!
        if (!(*curr).valid) {
            // update slot
            (*curr).tag = tag;
            (*curr).valid = true;
            (*curr).dirty = false;
            (*curr).load_ts = 0;
            (*curr).access_ts = 0;
            return evict_dirty;
        }
    }
    
    // choose which slot to evict & update evict_dirty
    uint32_t evict_index = chooseEvict(cache, index, lru);
    Slot *evict = &cache->sets[index].slots[evict_index];
    evict_dirty = evict->dirty;
    
    // update the slot info
    evict->tag = tag;
    evict->valid = true;
    evict->dirty = store;
    evict->load_ts = 0;
    evict->access_ts = 0;

    return evict_dirty;
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