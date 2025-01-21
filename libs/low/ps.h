enum FORK__STATE {
    FORK__SUCCESS = 0,
    FORK__FAILURE = -1,
};


pid_t PS__CLIENT_PIDS[1000];
size_t PS__CLIENT_RUNNING = 0;
pid_t PS__CASHIER_PID;
pid_t PS__LIFEGUARD_PIDS[POOL__NUM];


const char* PS__CLIENT_PATH = "build/client";
const char* PS__CLIENT_NAME  = "./build/client";

const char* PS__CASHIER_PATH = "build/cashier"; 
const char* PS__CASHIER_NAME  = "./build/cashier";

const char* PS__LIFEGUARD_PATH = "build/lifeguard"; 
const char* PS__LIFEGUARD_NAME  = "./build/lifeguard";