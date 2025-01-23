#pragma once

#include <pthread.h>


struct Child {
    pthread_t tid;

    int age;
    bool swim_cap_on;
    bool diaper_on;

    bool WAIT_IN_CASH;
    bool SWIM_IN_POOL;
} typedef Child;


struct Client {
    int age;
    bool swim_cap_on;
} typedef Client;