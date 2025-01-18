#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wait.h>
#include <unistd.h>

#include "random.h"
#include "pools.h"
#include "low/key.h"
#include "low/ps.h"
#include "key_ids.h"
#include "low/shared_mem.h"


void setup(){
    // Init shared mem
    key_t key;

    olimpic_pool.size = 0;
    key = get_key(OLIMPIC_POOL__KEY_ID);
    POOL[OLIMPIC] = access_shared_mem(key, sizeof(olimpic_pool), IPC_CREAT|0600);


    leisure_pool.size = 0;
    key = get_key(LEISURE_POOL__KEY_ID);
    POOL[LEISURE] = access_shared_mem(key, sizeof(leisure_pool), IPC_CREAT|0600);


    paddling_pool.size = 0;
    key = get_key(PADDLING_POOL__KEY_ID);
    POOL[PADDLING] = access_shared_mem(key, sizeof(paddling_pool), IPC_CREAT|0600);
}


void clean_up(){
    delete_shared_mem(POOL[OLIMPIC]);
    delete_shared_mem(POOL[LEISURE]);
    delete_shared_mem(POOL[PADDLING]);
}


int time_HHMM(int hour, int min){
    return hour * 60 + min;
}

int SIM_TIME = 1; // 1 real second =  SIM_TIME minutes in simulation 
int SIM_TIME_START;
int SIM_TIME_END;

int SPAWN_CLIENT_PERC = 47;

bool rand_client(){
    int spawn_client_perc = rand_int(0, 100);
    if(spawn_client_perc <= SPAWN_CLIENT_PERC){
        return true;
    }

    return false;
}

int main() {
    int SIM_TIME_START = time_HHMM(0, 0);
    int SIM_TIME_END = time_HHMM(23, 59);
    // setup();

    // pid_t ps[100];

    // for(int i = 0; i < 100; i++){
    //     pid_t pid;

    //     switch(pid = fork()){
    //         case FORK__FAIL:
    //             perror(__func__);
    //             exit(EXIT_FAILURE);

    //         case FORK__SUCCESS:
    //             ps[i] = pid;
    //             execl(CLIENT_PS_NAME, CLIENT_PS_RUN, NULL);
    //             perror(__func__);
    //             exit(EXIT_FAILURE);
    //     }
    // }


    // for(int i = 0; i < 100; i++){
    //     wait(&(ps[i]));
    // }

    int sim_time = SIM_TIME_START;

    while(sim_time != SIM_TIME_END + 1){
        printf("%d", sim_time);
        sim_time += SIM_TIME;

        if(rand_client()){
            printf(" - Client spawned!");
        }

        printf("\n");
        sleep(1);
    }


    printf("END\n");

    // clean_up();
    return 0;
}