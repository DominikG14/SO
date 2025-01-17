#pragma once


enum POOLS {
    OLIMPIC,
    LEISURE,
    PADDLING,
    POOLS__SIZE,
};

int POOL[POOLS__SIZE];


const int OLIMPIC_POOL__MAX_SIZE = 10;

struct OLIMPIC_POOL__SHARED_MEM {
    int size;
} typedef OlimpicPool;


const int LEISURE_POOL_MAX_SIZE  = 10;

struct LEISURE_POOL__SHARED_MEM {
    int size;
} typedef LeisurePool;


const int PADDLING_POOL_MAX_SIZE = 10;

struct PADDLING_POOL__SHARED_MEM {
    int size;
} typedef PaddlingPool;