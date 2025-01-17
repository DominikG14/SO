#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "pools.h"
#include "low/key.h"
#include "key_ids.h"
#include "low/shared_mem.h"


OlimpicPool  olimpic_pool;
LeisurePool  leisure_pool;
PaddlingPool paddling_pool;


void setup(){
    // Set random generator
    srand(time(0));


    // Init shared mem
    key_t key;

    olimpic_pool.size = 0;
    key = get_key(OLIMPIC_POOL__KEY_ID);
    POOL[OLIMPIC] = access_shared_mem(key, sizeof(olimpic_pool), IPC_CREAT|0600);


    leisure_pool.size = 0;
    key = get_key(LEISURE_POOL__KEY_ID);
    POOL[LEISURE] = access_shared_mem(key, sizeof(leisure_pool), IPC_CREAT|0600);


    paddling_pool.size = 0;
    key = get_key(PADDLING_POOL__KEY_ID);
    POOL[PADDLING] = access_shared_mem(key, sizeof(paddling_pool), IPC_CREAT|0600);
}


void clean_up(){
    delete_shared_mem(POOL[OLIMPIC]);
    delete_shared_mem(POOL[LEISURE]);
    delete_shared_mem(POOL[PADDLING]);
}


int rand_int(int min, int max){
    return min + rand() % (max - min + 1);
}


struct Client {
    int age;
    bool swim_cap_on;
    bool diaper_on;
} typedef Client;


int main() {
    setup();

    for(int i = 0; i < 100; i++){
        printf("%d\n", rand_int(0, 100));
    }

    clean_up();
    return 0;
}