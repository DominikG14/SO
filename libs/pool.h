#pragma once


enum POOL__TYPES {
    OLIMPIC,
    LEISURE,
    PADDLING,
    POOL__NUM,
};

int POOL__SHMID[POOL__NUM];

const int OLIMPIC_POOL__MAX_SIZE = 10;
const int LEISURE_POOL__MAX_SIZE  = 10;
const int PADDLING_POOL__MAX_SIZE = 10;


int POOL_SIZE[] = {
    OLIMPIC_POOL__MAX_SIZE,
    LEISURE_POOL__MAX_SIZE,
    PADDLING_POOL__MAX_SIZE,
};


char* OLIMPIC_POOL__TMP_FILE = "./tmp/op_shmid";
char* LEISURE_POOL__TMP_FILE = "./tmp/lp_shmid";
char* PADDLING_POOL__TMP_FILE = "./tmp/pp_shmid";


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