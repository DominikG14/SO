#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "random.h"
#include "pool.h"
#include "low/key.h"
#include "low/ps.h"
#include "low/signal.h"
#include "low/shared_mem.h"
#include "low/files.h"
#include "keys_id.h"


int SIM_TIME__PER_SEC = 30; // 1 real second = (SIM_TIME__PER_SEC) minutes in simulation 
int SIM_TIME__START;
int SIM_TIME__END;

int POOL__OPEN_TIME;
int POOL__CLOSE_TIME;

int SPAWN_CLIENT_PERC = 47;

int SIM_TIME__CURR;
bool POOL__IS_OPEN = false;


// Converts HH:MM time format to minutes
int time_HHMM(int hour, int min){
    return hour * 60 + min;
}


void setup(){
    // tmp folder
    mkdir("./tmp", 0700);

    // Signal init
    signal(SIGUSR1, SIG_IGN);
    key_t key;

    // Init shared mem
    char pool_shmid_str[FILE__SIZE];

    // Pools setup
    olimpic_pool.size = 0;
    key = get_key(OLIMPIC_POOL__KEY_ID);
    POOL__SHMID[OLIMPIC] = access_shared_mem(key, sizeof(olimpic_pool), IPC_CREAT|0600);
    snprintf(pool_shmid_str, sizeof(pool_shmid_str), "%d", POOL__SHMID[OLIMPIC]);
    write_file(OLIMPIC_POOL__TMP_FILE, pool_shmid_str);

    leisure_pool.size = 0;
    key = get_key(LEISURE_POOL__KEY_ID);
    POOL__SHMID[LEISURE] = access_shared_mem(key, sizeof(leisure_pool), IPC_CREAT|0600);
    snprintf(pool_shmid_str, sizeof(pool_shmid_str), "%d", POOL__SHMID[LEISURE]);
    write_file(LEISURE_POOL__TMP_FILE, pool_shmid_str);

    paddling_pool.size = 0;
    key = get_key(PADDLING_POOL__KEY_ID);
    POOL__SHMID[PADDLING] = access_shared_mem(key, sizeof(paddling_pool), IPC_CREAT|0600);
    snprintf(pool_shmid_str, sizeof(pool_shmid_str), "%d", POOL__SHMID[PADDLING]);
    write_file(PADDLING_POOL__TMP_FILE, pool_shmid_str);

    // Simulation time setup
    SIM_TIME__START = time_HHMM(9, 50);
    SIM_TIME__END = time_HHMM(13, 0);

    POOL__OPEN_TIME = time_HHMM(10, 0);
    POOL__CLOSE_TIME = time_HHMM(12, 30);

    SIM_TIME__CURR = SIM_TIME__START;
}


void clean_up(){
    delete_shared_mem(POOL__SHMID[OLIMPIC]);
    delete_shared_mem(POOL__SHMID[LEISURE]);
    delete_shared_mem(POOL__SHMID[PADDLING]);
}


bool rand_client(){
    int spawn_client_perc = rand_int(0, 100);
    if(spawn_client_perc <= SPAWN_CLIENT_PERC){
        return true;
    }

    return false;
}


void disp_time(){
    int hour = SIM_TIME__CURR / 60;
    int min = SIM_TIME__CURR % 60;

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
    execl(PS__CLIENT_PATH, PS__CLIENT_NAME, NULL);
    perror(__func__);
    exit(EXIT_FAILURE);
}


void open_cash(){
    pid_t pid;

    switch(pid = fork()){
        case FORK__FAILURE:
            perror(__func__);
            exit(EXIT_FAILURE);

        case FORK__SUCCESS:
            execl(PS__CASHIER_PATH, PS__CASHIER_NAME, NULL);
            perror(__func__);
            exit(EXIT_FAILURE);

        default:
            PS__CASHIER_PID = pid;
    }
}


void set_lifeguard(int pool_num){
    char str_pool_num[2];
    snprintf(str_pool_num, 2, "%d", pool_num);

    execl(PS__LIFEGUARD_PATH, PS__LIFEGUARD_NAME, str_pool_num, NULL);
    perror(__func__);
    exit(EXIT_FAILURE);
}


void close_cash(){
    send_signal(PS__CASHIER_PID, SIG__CLOSE_POOL);
    waitpid(PS__CASHIER_PID, NULL, 0);
}


void set_all_lifeguards(){
    pid_t pid;

    for(int pool_num = 0; pool_num < POOL__NUM; pool_num++){
        switch(pid = fork()){
            case FORK__FAILURE:
                perror(__func__);
                exit(EXIT_FAILURE);

            case FORK__SUCCESS:
                set_lifeguard(pool_num);

            default:
                PS__LIFEGUARD_PIDS[pool_num] = pid;
        }
    }
}


void remove_all_lifeguards(){
    for(int pool_num = 0; pool_num < POOL__NUM; pool_num++){
        send_signal(PS__LIFEGUARD_PIDS[pool_num], SIG__CLOSE_POOL);
        waitpid(PS__LIFEGUARD_PIDS[pool_num], NULL, 0);
    }
}


void remove_all_clients(){
    for(int i = 0; i < PS__CLIENT_RUNNING; i++){
        send_signal(PS__CLIENT_PIDS[i], SIG__CLOSE_POOL);
        waitpid(PS__CLIENT_PIDS[i], NULL, 0);
    }
}


int main() {
    setup();

    pid_t pid;

    while(SIM_TIME__CURR <= SIM_TIME__END){
        disp_time();


        // Open pool
        if(!POOL__IS_OPEN && SIM_TIME__CURR >= POOL__OPEN_TIME && SIM_TIME__CURR <= POOL__CLOSE_TIME){
            POOL__IS_OPEN = true;
            open_cash();
            set_all_lifeguards();
            sleep(1); // Wait for staff to setup
        }


        // Close pool
        if(POOL__IS_OPEN && SIM_TIME__CURR > POOL__CLOSE_TIME){
            remove_all_clients();
            close_cash();
            remove_all_lifeguards();
            POOL__IS_OPEN = false;
        }


        // Clients
        if(SIM_TIME__CURR < POOL__OPEN_TIME || SIM_TIME__CURR > POOL__CLOSE_TIME){
            printf(" - Pool closed!");
        }

        else if(rand_client()){
            switch(pid = fork()){
                case FORK__FAILURE:
                    perror(__func__);
                    exit(EXIT_FAILURE);

                case FORK__SUCCESS:
                    spawn_client();
                
                default:
                    PS__CLIENT_PIDS[PS__CLIENT_RUNNING] = pid;
                    PS__CLIENT_RUNNING++;
            }
        }


        // Time
        printf("\n");
        sleep(1);
        SIM_TIME__CURR += SIM_TIME__PER_SEC;
    }

    clean_up();
    return 0;
}