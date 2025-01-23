#pragma once

enum POOL__TYPES {
    OLIMPIC,
    LEISURE,
    PADDLING,
    POOL_NUM,
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