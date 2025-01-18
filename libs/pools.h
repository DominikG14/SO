#pragma once


enum POOLS {
    OLIMPIC,
    LEISURE,
    PADDLING,
    POOLS__SIZE,
};

int POOL[POOLS__SIZE];

const int OLIMPIC_POOL__MAX_SIZE = 10;
const int LEISURE_POOL_MAX_SIZE  = 10;
const int PADDLING_POOL_MAX_SIZE = 10;


int POOL_SIZE[] = {
    OLIMPIC_POOL__MAX_SIZE,
    LEISURE_POOL_MAX_SIZE,
    PADDLING_POOL_MAX_SIZE,
};


struct OLIMPIC_POOL__SHARED_MEM {
    int size;
} typedef OlimpicPool;


struct LEISURE_POOL__SHARED_MEM {
    int size;
} typedef LeisurePool;


struct PADDLING_POOL__SHARED_MEM {
    int size;
} typedef PaddlingPool;


// Global pools objects
OlimpicPool  olimpic_pool;
LeisurePool  leisure_pool;
PaddlingPool paddling_pool;