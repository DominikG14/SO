#pragma once


int POOL_LEISURE_AGE_AVG = 40;


enum POOL__TYPES {
    OLIMPIC,
    LEISURE,
    PADDLING,
    POOL_NUM,
};

const int POOL_OLIMPIC_MAX_SIZE  = 3;
const int POOL_LEISURE_MAX_SIZE  = 10;
const int POOL_PADDLING_MAX_SIZE = 2;


int POOL_SIZE[] = {
    POOL_OLIMPIC_MAX_SIZE,
    POOL_LEISURE_MAX_SIZE,
    POOL_PADDLING_MAX_SIZE,
};


struct POOL_OLIMPIC_SHARED_MEM {
    int size;
} typedef OlimpicPool;


struct POOL_LEISURE_SHARED_MEM {
    int size;
    int age_sum;
} typedef LeisurePool;


struct POOL_PADDLING_SHARED_MEM {
    int size;
} typedef PaddlingPool;


int POOL_SHARED_MEM_SIZE[] = {
    sizeof(struct POOL_OLIMPIC_SHARED_MEM),
    sizeof(struct POOL_LEISURE_SHARED_MEM),
    sizeof(struct POOL_PADDLING_SHARED_MEM),
};