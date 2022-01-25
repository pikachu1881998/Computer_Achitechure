/**
 * @file cache.c
 * @brief CS{4/6}290 / ECE{4/6}100 Spring 2021 Project 1 cache simulator
 *
 * Fill in the functions to complete the cache simulator
 *
 * @author <Your name goes here>
 */

#include <cstdarg>
#include <cinttypes>
#include <cstdio>
#include <cstdbool>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include "cache.hpp"

// Use this for printing errors while debugging your code
// Most compilers support the __LINE__ argument with a %d argument type
// Example: print_error_exit("Error: Could not allocate memory %d", __LINE__);
static inline void print_error_exit(const char *msg, ...) {
    va_list argptr;
    va_start(argptr, msg);
    fprintf(stderr, msg, argptr);
    va_end(argptr);
    exit(EXIT_FAILURE);
}

// Define data structures and globals you might need for simulating the cache hierarchy below

// TODO
cache_set_t *cache;
cache_set_t *victimCache;
uint64_t offsetBits;
uint64_t indexBits;
uint64_t tagBits;
uint64_t num_sets;           // Number of sets
uint64_t blocksize;
uint64_t cachesize;
uint64_t num_ways;
uint64_t vcSize;

uint64_t clock;





// todo checek the minimum frequency of all the available block that is not mru and return that index
// so inside the for loop check if i!=MRUway;

/**
 * least frequently used block in a set but not the most recently used block
 *
 * @param set : a array of struct blocks which we are tring to find the lfu for
 * @return  returns index of the least recently used block
 *
 */

uint64_t lru_stack_get_lfu(cache_set_t *set) {
    uint64_t indextoreturn = 0;                                     // which way is hoding the lfu block in this paticular set
    uint64_t min = 0;                                               // min freq
    if (set->size != 1) {                                           // this is due to direct map since it has only one block it has to return the most recently used block
        if (set->MRU->way != set->blocks[0].way) {                  // checking if the first block is not the MRU
            min = set->blocks[0].freqCount;
            indextoreturn = 0;
        } else {                                                    // if it is then set somthing else as the basline to compare it with others
            min = set->blocks[1].freqCount;
            indextoreturn = 1;
        }
        for (int i = 0; i < set->size; i++) {                                                                      //comparing if the block has lower frequency count then other and
            if (min > set->blocks[i].freqCount && set->MRU->tag != set->blocks[i].tag) {                           //fi yes then check if its not an MRU if so the update the baseline
                min = set->blocks[i].freqCount;
                indextoreturn = i;
            } else if (min == set->blocks[i].freqCount && set->MRU->tag != set->blocks[i].tag) {                   // if not lower than check if its equal if yes than set the higher tag to
                if (set->blocks[indextoreturn].tag < set->blocks[i].tag) {                                         // the LFU block and return its index
                    min = set->blocks[i].freqCount;
                    indextoreturn = i;
                }
            }
        }
    }

    return indextoreturn;
}

/**FIrsin block gets out Firtst
 *
 * @param set set og blocks with the firstIN timestamp
 * @return give the index of the block that came first
 */
uint64_t lru_stack_get_fifo(cache_set_t *set) {
    /*
     * setting up the base line
     * set the first block to be the block whith the smallest time stamp
     */
    uint64_t min = set->blocks[0].firstIN;
    uint64_t indextoreturn = 0;
    // checking it its not a direct map since it has only one block to it
    if (set->size != 1) {
        /*
         * looping though the set to find the minimum time stamp
         */
        for (int i = 0; i < set->size; i++) {
            if (min > set->blocks[i].firstIN) {
                min = set->blocks[i].firstIN;
                indextoreturn = i;
            }
        }
    }
    return indextoreturn;
}
/**least recenty usd block
 *
 * @param set set set of blocks with the timestamp
 * @return index of the block with the smallest timestamp
 */
uint64_t lru_stack_get_lru(cache_set_t *set) {
    /*
     * setting up the baseline
     * setting the first block to be the LRU and then checking it with other blocks
     */
    uint64_t min = set->blocks[0].timestamp;
    uint64_t indextoreturn = 0;
    if (set->size != 1) {
        for (int i = 0; i < set->size; i++) {
            if (min > set->blocks[i].timestamp) {
                min = set->blocks[i].timestamp;
                indextoreturn = i;
            }
        }
    }
    return indextoreturn;
}

/**
 * Function to initialize any data structures you might need for simulating the cache hierarchy. Use
 * the sim_conf structure for initializing dynamically allocated memory.
 *
 * @param sim_conf Pointer to simulation configuration structure
 *
 */
void sim_init(struct sim_config_t *sim_conf) {
    // TODO from c, b, s calculate the block size, cache size and number of sets,
    // TODO get the tag, index and offset bits
    // TODO allocate the space for the size of cache
    // TODO check if victim cache is enabled if so allocate the space for it based on the number of V_blocks
    // TODO create the sets in cache if its a set associative cache
    // TODO allocate all the blocks in cache
    // TODO assign each block valid=0, dirty=0, and tag = 0

    int b = sim_conf->l1data.b;
    int s = sim_conf->l1data.s;
    int c = sim_conf->l1data.c;
    offsetBits = b;
    indexBits = c - b - s;
    tagBits = 64 - b - indexBits;
    num_sets = pow(2, indexBits);
    num_ways = pow(2, s);
    cachesize = pow(2, c);
    blocksize = pow(2, b);
    clock = 0;

    /*
     * creating the cache with the number of sets and the blocks
     */
    cache = (cache_set_t *) malloc(num_sets * sizeof(cache_set_t));
    /*
     * Allocationg space in memory for the blocks
     * assigning the initial values to the properties og the block
     */
    for (int i = 0; i < num_sets; i++) {
        cache[i].blocks = (cache_block_t *) malloc(num_ways * blocksize * sizeof(cache_block_t));
        cache[i].size = num_ways;
        cache[i].MRU = &cache[i].blocks[0];
        for (int j = 0; j < num_ways; j++) {
            cache[i].blocks[j].dirty = 0;
            cache[i].blocks[j].valid = 0;
            cache[i].blocks[j].tag = 0;
            cache[i].blocks[j].freqCount = 0;
            cache[i].blocks[j].timestamp = 0;
        }
    }
    /*
     * check if the victim cache exists them allocate memoty for it too same way
     * since there is only one set wejust have to allocate for the blocks
     */
    if (sim_conf->has_victim_cache) {
        victimCache = (cache_set_t *) malloc(sizeof(cache_set_t));
        vcSize = sim_conf->V;
        victimCache->blocks = (cache_block_t *) malloc(vcSize * sizeof(cache_block_t));
        victimCache->size = sim_conf->V;

        for (int i = 0; i < vcSize; i++) {
            victimCache->blocks[i].valid = 0;
            victimCache->blocks[i].dirty = 0;
            victimCache->blocks[i].tag = 0;
            victimCache->blocks[i].freqCount = 0;
            victimCache->blocks[i].timestamp = 0;
        }
    }


}


/**
 * Function to perform cache accesses, one access at a time. The print_debug function should be called
 * if the debug flag is true
 *
 * @param addr The address being accessed by the processor
 * @param type The type of access - Load (L) or Store (S)
 * @param sim_stats Pointer to simulation statistics structure - Should be populated here
 * @param sim_conf Pointer to the simulation configuration structure - Don't modify it in this function
 */
void cache_access(uint64_t addr, char type, struct sim_stats_t *sim_stats, struct sim_config_t *sim_conf) {

    // TODO from address extract the tag, index and offset
    // TODO compare the tag in l1 cache
    // TODO compare tag in victim cache
    // TODO if we find it in l1 and if its valid hit++
    // TODO if we find it in VC and if its valid Vhit++
    // TODO if hit was in vc than promote it form the VC to L1 and e
    //      victim a block form l1 to VC according to the replacement policy
    // TODO if its a VC and L1 miss update the miss stats
    // TODO check if what type of access it is to update dity and valid bits
    // TODO update stats for l1 cache
    // TODO update the stats for VC
    //


    /*
     * global clock for the timestamps
     */
    clock++;
    sim_stats->l1data_num_accesses++;
    //based ont the access type update the stats
    if (type == STORE) {
        sim_stats->l1data_num_accesses_stores++;
    } else
        sim_stats->l1data_num_accesses_loads++;

    uint64_t tag = (addr / blocksize) / num_sets; //tag of the cache block
    uint64_t index = (addr / blocksize) % num_sets;//set of the block
    /////////////////////////////////
    //// this is to check l1 hit ////
    ////////////////////////////////

    /*
     * checking if hte blocks tag maches the access tag and if the block is valid
     * if those condition met than its a hit else its a l1 miss
     */
    for (int i = 0; i < num_ways; i++) {
        //if its a hit update the block properties according
        if (tag == cache[index].blocks[i].tag && cache[index].blocks[i].valid == 1) {
            cache[index].blocks[i].freqCount++;
            cache[index].MRU = &cache[index].blocks[i];
            cache[index].blocks[i].timestamp = clock;
            cache[index].blocks[i].way = i;
            if (type == STORE) {
                cache[index].blocks[i].dirty = 1;
            }// todo uer the rp to get the corrct replacemaent policy
            return;
        }
    }
    // based ont the type of the access update the stats
    sim_stats->l1data_num_misses++;
    if (type == STORE) {
        sim_stats->l1data_num_misses_stores++;
    } else
        sim_stats->l1data_num_misses_loads++;

    // l1 misses
    // look into the VC if it exists
    // todo update the stats
    // todo ask how to update the frequency
    // does the vc promotion does this means i have to update the lru for both and increment the frequency for the block.
    /////////////////////
    /// l1 is VC hit ///
    ////////////////////

    if (sim_conf->has_victim_cache) {
        sim_stats->victim_cache_accesses++;
        /*
         * calculating the VC tag since it doset't have index bits it has longer TAG for the blocks
         */
        uint64_t TagVc = (tag << indexBits) + index;
        uint64_t blockhitIndex = 0;
        uint64_t evictedWay = 0;
        bool hitVC = false;
        /*
         *  checking if the TAG in vc matches the tag and if the VC block is valid or not
         *  if its valid then its a hit get the index and do the swaping
         */
        for (int i = 0; i < vcSize; i++) {
            if (TagVc == victimCache->blocks[i].tag && victimCache->blocks[i].valid == 1) {
                sim_stats->victim_cache_hits++;
                blockhitIndex = i;
                hitVC = true;
                break;
            }
        }
        /*
         * for the swaping use one of the three Rp to get the block to evict from the cache
         * since we are evcting the block update the stats
         */
        if (hitVC) {
            sim_stats->l1data_num_evictions++;
            switch (sim_conf->rp) {
                case 1:
                    evictedWay = lru_stack_get_lru(&cache[index]);
                    break;
                case 2:
                    evictedWay = lru_stack_get_lfu(&cache[index]);
                    break;
                case 3:
                    evictedWay = lru_stack_get_fifo(&cache[index]);
                    break;
                default :
                    evictedWay = lru_stack_get_lru(&cache[index]);
                    break;
            }
            // todo make sure you make a hard copy not the soft copy
            /*
             * swapping the block between vc and the l1 cache
             * since getting the block consider the fetching from the memory update all the time stamps
             * and reset the frequency of the bloc that is being promoted
             * preserve the dirty bit of the block being promoted
             */

            cache_block_t temp = victimCache->blocks[blockhitIndex];
            victimCache->blocks[blockhitIndex] = cache[index].blocks[evictedWay];
            victimCache->blocks[blockhitIndex].tag = (cache[index].blocks[evictedWay].tag << indexBits) + index;
            victimCache->blocks[blockhitIndex].dirty = cache[index].blocks[evictedWay].dirty;
            victimCache->blocks[blockhitIndex].firstIN = clock;
            //// since it arrives in the first time it should be reset

            cache[index].blocks[evictedWay] = temp;
            cache[index].blocks[evictedWay].tag = temp.tag >> indexBits;
            cache[index].blocks[evictedWay].way = evictedWay;
            cache[index].blocks[evictedWay].firstIN = clock;
            cache[index].blocks[evictedWay].timestamp = clock;
            cache[index].blocks[evictedWay].freqCount = 0;
            cache[index].MRU = &cache[index].blocks[evictedWay];/////assuming as its coming from the memory so reset the in time
            // based on the type change the dirty bit of the block that is being promoted to the l1 cache
            if (type == STORE) {
                cache[index].blocks[evictedWay].dirty = 1;
            }
            return;
        }

    }



/*
 * if l1 and VC miss
 * update the stats for misses
 */
    if (sim_conf->has_victim_cache) {
        sim_stats->victim_cache_misses++;
    }


/**
 * if l1 and vc misses
 * todo get the block to evict from the l1's set
 * todo send that block to victim cache if valid
 * todo set the tag to the block in the set at
 * todo update the stack as recent access
 * todo based on the access type change the valid and dirty bit
 * todo update the stats
 */
///////////////////////////
///L1 Miss and VC miss ///
/////////////////////////


//////have to check if there is any empty blocks available to put in to the set and move onn

    for (int i = 0; i < cache[index].size; i++) {
        if (cache[index].blocks[i].valid != 1) {
            cache[index].blocks[i].valid = 1;
            cache[index].blocks[i].tag = tag;
            cache[index].blocks[i].freqCount = 0;
            cache[index].blocks[i].firstIN = clock;
            cache[index].blocks[i].timestamp = clock;
            cache[index].MRU = &cache[index].blocks[i];
            cache[index].blocks[i].way = i;
            sim_stats->bytes_transferred_from_mem += pow(2, offsetBits);
// based on the access type update dirty
            if (type == STORE) {
                cache[index].blocks[i].dirty = 1;
            } else {
                cache[index].blocks[i].dirty = 0;
            }

            return;
        }
    }
    /*
     * cache set is full and have to evict one block use the provided Rp to chose which block to evict
     */
    int evictedWay = 0;
    switch (sim_conf->rp) {
        case LRU:
            evictedWay = lru_stack_get_lru(&cache[index]);
            break;
        case LFU:
            evictedWay = lru_stack_get_lfu(&cache[index]);
            break;
        case FIFO:
            evictedWay = lru_stack_get_fifo(&cache[index]);
            break;
        default :
            evictedWay = lru_stack_get_lru(&cache[index]);
            break;
    }

    // update the stas
    sim_stats->l1data_num_evictions++;                                                              // if the block is not empty than its an eviction?


/*
 * checking if the VC exist
 * if yes than get the LRU block and
 */

//todo update stat
// if the evicted block is dirty than should we writeback or just sent to the VC with the dirty bit 1
// if VC exist
/*
 * if it exist then check if there is an empty block exist if not figure out what to evict and update the stats
 */
    if (sim_conf->has_victim_cache) {
        {
//todo check if the victim cache has a empty slot if yes than put it there else evict one of the victim cache block

            for (int i = 0; i < vcSize; i++) {
                if (victimCache->blocks[i].valid != 1) {
                    victimCache->blocks[i] = cache[index].blocks[evictedWay];
                    victimCache->blocks[i].tag = (cache[index].blocks[evictedWay].tag << indexBits) + index;
                    victimCache->blocks[i].valid = 1;
                    victimCache->blocks[i].freqCount = 0;
                    victimCache->blocks[i].firstIN = clock;
                    // getting the block from the memory and putting it black in the l1 cache
                    // updating the timestams and friquencies
                    cache[index].blocks[evictedWay].valid = 1;
                    cache[index].blocks[evictedWay].tag = tag;
                    cache[index].blocks[evictedWay].freqCount = 0;
                    cache[index].blocks[evictedWay].firstIN = clock;
                    cache[index].blocks[evictedWay].timestamp = clock;
                    cache[index].MRU = &cache[index].blocks[evictedWay];
                    cache[index].blocks[evictedWay].way = evictedWay;
                    sim_stats->bytes_transferred_from_mem += pow(2, offsetBits);
// based on the access type update dirty
                    if (type == STORE) {
                        cache[index].blocks[evictedWay].dirty = 1;
                    } else {
                        cache[index].blocks[evictedWay].dirty = 0;
                    }
                    return;
                }

            }

            /*
             * if VC is full seclect the bloc to evict and do the same as above essentially
             */
            uint64_t block_to_remove = lru_stack_get_fifo(
                    victimCache);// block with least recently used in victim cache// todo use the rp to use correct replacement policy

            // updat the stats
            if (victimCache->blocks[block_to_remove].dirty == 1) {
                sim_stats->num_writebacks++;
                sim_stats->bytes_transferred_to_mem += pow(2,
                                                           offsetBits);                                              // writing back the block that is being trnsferd to the meme//todo should we consider this as cache to mem byte
            }
            victimCache->blocks[block_to_remove].tag = (cache[index].blocks[evictedWay].tag << indexBits) +
                                                       index;                          // todo reccalculate the tag // updating the block info form the l1 to the Victim
            victimCache->blocks[block_to_remove].valid = 1;
            victimCache->blocks[block_to_remove].dirty = cache[index].blocks[evictedWay].dirty;
            victimCache->blocks[block_to_remove].freqCount = 0;
            victimCache->blocks[block_to_remove].firstIN = clock;
        }
    }
//if vitim doent exist than write to memory
    else if (cache[index].blocks[evictedWay].valid == 1 && cache[index].blocks[evictedWay].dirty == 1) {
        sim_stats->bytes_transferred_to_mem += pow(2, offsetBits);
        sim_stats->num_writebacks++;
    }
// updating the bloc that just became available
//assign tag and make it valid since we are bring it
    cache[index].blocks[evictedWay].valid = 1;
    cache[index].blocks[evictedWay].tag = tag;
    cache[index].blocks[evictedWay].freqCount = 0;
    cache[index].blocks[evictedWay].firstIN = clock;
    cache[index].blocks[evictedWay].timestamp = clock;
    cache[index].MRU = &cache[index].blocks[evictedWay];
    cache[index].blocks[evictedWay].way = evictedWay;
    sim_stats->bytes_transferred_from_mem += pow(2, offsetBits);
// based on the access type update dirty
    if (type == STORE) {
        cache[index].blocks[evictedWay].
                dirty = 1;
    } else {
        cache[index].blocks[evictedWay].
                dirty = 0;
    }
//todo use the switch case to use differnt rp policy

    return;


}


/**
 * Function to cleanup dynamically allocated simulation memory, and perform any calculations
 * that might be required
 *
 * @param stats Pointer to the simulation structure - Final calculations should be performed here
 */
void sim_cleanup(struct sim_stats_t *sim_stats, struct sim_config_t *sim_conf) {
    // TODO calculate the times and the stats
    //
    //
    /*
     * calculate the miss rates of l1 and VC
     * calculate the AAT for the cache
     * deallocate the CAcahe and VC if exsists
     */
    sim_stats->l1data_miss_penalty = 60.0;
    sim_stats->l1data_miss_rate = (double) sim_stats->l1data_num_misses / sim_stats->l1data_num_accesses;
    if (sim_conf->has_victim_cache) {
        sim_stats->victim_cache_miss_rate = (double) sim_stats->victim_cache_misses /
                                            sim_stats->victim_cache_accesses;
        sim_stats->avg_access_time = (double) sim_stats->l1data_hit_time +
                                     (sim_stats->victim_cache_miss_rate * sim_stats->l1data_miss_rate *
                                      sim_stats->l1data_miss_penalty);
    } else
        sim_stats->avg_access_time = (double) sim_stats->l1data_hit_time +
                                     (sim_stats->l1data_miss_rate *
                                      sim_stats->l1data_miss_penalty);
    for (int i = 0; i < num_sets; i++) {
        free(cache[i].blocks);

    }
    free(cache);
    if (sim_conf->has_victim_cache) {
        free(victimCache->blocks);
        free(victimCache);
    }
}
