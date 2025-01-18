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


int SIM_TIME = 7; // 1 real second = (SIM_TIME) minutes in simulation 
int SIM_TIME_START;
int SIM_TIME_END;

int POOL_OPEN;
int POOL_CLOSE;

int SPAWN_CLIENT_PERC = 47;

int sim_time;


// Converts HH:MM time format to minutes
int time_HHMM(int hour, int min){
    return hour * 60 + min;
}


void setup(){
    // Init shared mem
    key_t key;

    // Pools setup
    olimpic_pool.size = 0;
    key = get_key(OLIMPIC_POOL__KEY_ID);
    POOL[OLIMPIC] = access_shared_mem(key, sizeof(olimpic_pool), IPC_CREAT|0600);

    leisure_pool.size = 0;
    key = get_key(LEISURE_POOL__KEY_ID);
    POOL[LEISURE] = access_shared_mem(key, sizeof(leisure_pool), IPC_CREAT|0600);

    paddling_pool.size = 0;
    key = get_key(PADDLING_POOL__KEY_ID);
    POOL[PADDLING] = access_shared_mem(key, sizeof(paddling_pool), IPC_CREAT|0600);

    // Simulation time setup
    SIM_TIME_START = time_HHMM(9, 20);
    SIM_TIME_END = time_HHMM(12, 0);

    POOL_OPEN = time_HHMM(10, 0);
    POOL_CLOSE = time_HHMM(11, 20);

    sim_time = SIM_TIME_START;
}


void clean_up(){
    delete_shared_mem(POOL[OLIMPIC]);
    delete_shared_mem(POOL[LEISURE]);
    delete_shared_mem(POOL[PADDLING]);
}


bool rand_client(){
    int spawn_client_perc = rand_int(0, 100);
    if(spawn_client_perc <= SPAWN_CLIENT_PERC){
        return true;
    }

    return false;
}


void disp_time(){
    int hour = sim_time / 60;
    int min = sim_time % 60;

    if(hour < 10){
        printf("0");
    }
    printf("%d:", hour);

    if(min < 10){
        printf("0");
    }
    printf("%d", min);
}


void spawn_client(){
    execl(CLIENT_PS_NAME, CLIENT_PS_RUN, NULL);
    perror(__func__);
    exit(EXIT_FAILURE);
}


int main() {
    setup();

    pid_t pid;
    pid_t ps[1000];
    size_t created_ps = 0;


    while(sim_time <= SIM_TIME_END){
        disp_time();

        if(sim_time < POOL_OPEN || sim_time > POOL_CLOSE){
            printf(" - Pool closed!");
        }

        else if(rand_client()){
            switch(pid = fork()){
                case FORK__FAIL:
                    perror(__func__);
                    exit(EXIT_FAILURE);

                case FORK__SUCCESS:
                    ps[created_ps] = pid;
                    created_ps++;
                    spawn_client();
            }
        }

        printf("\n");
        sleep(1);
        sim_time += SIM_TIME;
    }


    // Wait for all clients to leave the pool complex
    for(int i = 0; i < created_ps; i++){
        wait(&(ps[i]));
    }


    printf("END\n");

    clean_up();
    return 0;
}