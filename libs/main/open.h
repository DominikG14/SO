#pragma once

#include <sys/types.h>

#include "random.h"
#include "logging.h"
#include "pool.h"
#include "low.h"
#include "vars.h"


// Lifeguards
void set_lifeguard(int pool_num){
    char str_pool_num[2];
    sprintf(str_pool_num, "%d", pool_num);

    execl(PS_LIFEGUARD_PATH, PS_LIFEGUARD_NAME, str_pool_num, NULL);
    perror(__func__);
    exit(EXIT_FAILURE);
}


void set_all_lifeguards(){
    pid_t pid;

    for(int pool_num = 0; pool_num < POOL_NUM; pool_num++){
        switch(pid = fork()){
            case FORK_FAILURE:
                perror(__func__);
                exit(EXIT_FAILURE);

            case FORK_SUCCESS:
                set_lifeguard(pool_num);

            default:
                LIFEGUARD_PIDS[pool_num] = pid;
        }
    }
}


// Cashier
void open_cash(){
    pid_t pid;

    switch(pid = fork()){
        case FORK_FAILURE:
            perror(__func__);
            exit(EXIT_FAILURE);

        case FORK_SUCCESS:
            execl(PS_CASHIER_PATH, PS_CASHIER_NAME, NULL);
            perror(__func__);
            exit(EXIT_FAILURE);

        default:
            CASHIER_PID = pid;
    }
}


// Clients
bool rand_client(){
    int spawn_client_perc = rand_int(0, 100);
    if(spawn_client_perc <= CLIENT_SPAWN_PERC){
        return true;
    }

    return false;
}


void spawn_client(){
    execl(PS_CLIENT_PATH, PS_CLIENT_NAME, NULL);
    perror(__func__);
    exit(EXIT_FAILURE);
}


void let_clients_in(){
    pid_t pid;

    if(!COMPLEX_IS_OPEN){
        log_console(getpid(),
            WHO__POOL_COMPLEX,
            ACTION__CLOSED,
            LOCATION__POOL_COMPLEX,
            REASON__NONE
        );
        return;
    }

    if(rand_client()){
        switch(pid = fork()){
            case FORK_FAILURE:
                perror(__func__);
                exit(EXIT_FAILURE);

            case FORK_SUCCESS:
                spawn_client();
            
            default:
                CLIENT_PIDS[CLIENTS_NUM] = pid;
                CLIENTS_NUM++;
        }
    }
}


// Complex
void open_complex(){
    log_console(getpid(),
        WHO__POOL_COMPLEX,
        ACTION__OPENED,
        LOCATION__POOL_COMPLEX,
        REASON__NONE
    );

    COMPLEX_IS_OPEN = true;
    open_cash();
    set_all_lifeguards();
    sleep(1); // Wait for staff to setup
}