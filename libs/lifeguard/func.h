#pragma once

#include <stdbool.h>

#include "vars.h"
#include "low.h"
#include "random.h"
#include "pool.h"


void setup(){
    handle_signal(SIG_CLOSE_POOL, remove_lifeguard);

    key_t key = get_key(GUARDED_POOL);
    POOL_SHMID = access_shared_mem(key, POOL_NUM, 0200);
}


void close_pool(){
    switch(GUARDED_POOL){
        case OLIMPIC:
            OlimpicPool* pool =(OlimpicPool*) get_shared_mem();
            break;
        
        case LEISURE:
            break;

        case PADDLING:
            break;
    }
}


void remove_lifeguard(){
    LOG_leave_guarder_pool();
    LOG_leave_complex();
}