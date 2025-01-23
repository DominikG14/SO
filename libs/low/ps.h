#include "pool.h"


enum FORK__STATE {
    FORK_SUCCESS = 0,
    FORK_FAILURE = -1,
};


pid_t CLIENT_PIDS[1000];
size_t CLIENTS_NUM = 0;
pid_t CASHIER_PID;
pid_t LIFEGUARD_PIDS[POOL_NUM];


const char* PS_CLIENT_PATH = "build/client";
const char* PS_CLIENT_NAME  = "./build/client";

const char* PS_CASHIER_PATH = "build/cashier"; 
const char* PS_CASHIER_NAME  = "./build/cashier";

const char* PS_LIFEGUARD_PATH = "build/lifeguard"; 
const char* PS_LIFEGUARD_NAME  = "./build/lifeguard";