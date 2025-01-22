#pragma once


enum POOL__TYPES {
    OLIMPIC,
    LEISURE,
    PADDLING,
    POOL_NUM,
};

const size_t POOL_OLIMPIC_MAX_SIZE  = 10;
const size_t POOL_LEISURE_MAX_SIZE  = 10;
const size_t POOL_PADDLING_MAX_SIZE = 2;


size_t POOL_SIZE[] = {
    POOL_OLIMPIC_MAX_SIZE,
    POOL_LEISURE_MAX_SIZE,
    POOL_PADDLING_MAX_SIZE,
};


struct POOL_OLIMPIC_SHARED_MEM {
    size_t size;
} typedef OlimpicPool;


struct POOL_LEISURE_SHARED_MEM {
    size_t size;
    size_t age_sum;
} typedef LeisurePool;


struct POOL_PADDLING_SHARED_MEM {
    size_t size;
} typedef PaddlingPool;


size_t POOL_SHARED_MEM_SIZE[] = {
    sizeof(struct POOL_OLIMPIC_SHARED_MEM),
    sizeof(struct POOL_LEISURE_SHARED_MEM),
    sizeof(struct POOL_PADDLING_SHARED_MEM),
};