#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "random.h"
#include "pool.h"
#include "keys_id.h"
#include "utils.h"
#include "color.h"
#include "logging.h"

#include "low/key.h"
#include "low/sem.h"
#include "low/ps.h"
#include "low/signal.h"
#include "low/shared_mem.h"
#include "low/files.h"


int TIME_PER_SEC = 10; // 1 real second = (TIME_PER_SEC) minutes in simulation 
int TIME_START;
int TIME_END;

int POOL_OPEN_TIME;
int POOL_CLOSE_TIME;

int SPAWN_CLIENT_PERC = 100;

int TIME_CURR;
bool POOL_IS_OPEN = false;


void __create_pool_resources(int pool_num){
    key_t key;
    int semid;

    key = get_key(pool_num);
    access_shared_mem(key, POOL_SHARED_MEM_SIZE[pool_num], IPC_CREAT|0600);
    semid = access_sem(key, 2, IPC_CREAT|0600);
    init_sem(semid, 2);
}


void __create_tmp_dir(){
    switch(fork()){
        case FORK_FAILURE:
            perror(__func__);
            exit(EXIT_FAILURE);
        
        case FORK_SUCCESS:
            const char *cmd = "/bin/rm";
            char *args[] = {"rm", "-r", "./tmp", NULL};
            execv(cmd, args);
            perror(__func__);
            exit(EXIT_FAILURE);
    }
    sleep(1); // Wait for './tmp' dir to be deleted first
    mkdir("./tmp", 0700);
}

void __setup_simulation_time(){
    TIME_START = time_HHMM(9, 50);
    TIME_END = time_HHMM(14, 0);

    POOL_OPEN_TIME = time_HHMM(10, 0);
    POOL_CLOSE_TIME = time_HHMM(13, 30);

    TIME_CURR = TIME_START;
}

void __delete_pool_resources(int pool_num){
    key_t key;
    int shmid;
    int semid;

    key = get_key(pool_num);
    shmid = access_shared_mem(key, POOL_SHARED_MEM_SIZE[pool_num], 0600);
    delete_shared_mem(shmid);

    semid = access_sem(key, 2, 0600);
    delete_sem(semid);
}


void setup(){
    __create_tmp_dir();
    signal(SIGUSR1, SIG_IGN); // Signal init
    __setup_simulation_time();
    __create_pool_resources(OLIMPIC);
    __create_pool_resources(LEISURE);
    __create_pool_resources(PADDLING);
}


void clean_up(){
    __delete_pool_resources(OLIMPIC);
    __delete_pool_resources(LEISURE);
    __delete_pool_resources(PADDLING);
}


void disp_time(){
    int hour = TIME_CURR / 60;
    int min = TIME_CURR % 60;

    cyan();
    printf("\n");
    if(hour < 10){
        printf("0");
    }
    printf("%d:", hour);

    if(min < 10){
        printf("0");
    }
    printf("%d\n", min);
    reset();
}


bool rand_client(){
    int spawn_client_perc = rand_int(0, 100);
    if(spawn_client_perc <= SPAWN_CLIENT_PERC){
        return true;
    }

    return false;
}


void spawn_client(){
    execl(PS_CLIENT_PATH, PS_CLIENT_NAME, NULL);
    perror(__func__);
    exit(EXIT_FAILURE);
}


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


void set_lifeguard(int pool_num){
    char str_pool_num[2];
    sprintf(str_pool_num, "%d", pool_num);

    execl(PS_LIFEGUARD_PATH, PS_LIFEGUARD_NAME, str_pool_num, NULL);
    perror(__func__);
    exit(EXIT_FAILURE);
}


void close_cash(){
    send_signal(CASHIER_PID, SIG_CLOSE_POOL);
    waitpid(CASHIER_PID, NULL, 0);
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


void remove_all_lifeguards(){
    for(int pool_num = 0; pool_num < POOL_NUM; pool_num++){
        send_signal(LIFEGUARD_PIDS[pool_num], SIG_CLOSE_POOL);
        waitpid(LIFEGUARD_PIDS[pool_num], NULL, 0);
    }
}


void remove_all_clients(){
    for(int i = 0; i < CLIENTS_NUM; i++){
        send_signal(CLIENT_PIDS[i], SIG_CLOSE_POOL);
        waitpid(CLIENT_PIDS[i], NULL, 0);
    }
}


int main() {
    setup();

    pid_t pid;

    while(TIME_CURR <= TIME_END){
        disp_time();

        // Open pool
        if(!POOL_IS_OPEN && TIME_CURR >= POOL_OPEN_TIME && TIME_CURR <= POOL_CLOSE_TIME){
            log_console(getpid(),
                WHO__POOL_COMPLEX,
                ACTION__OPENED,
                LOCATION__POOL_COMPLEX,
                REASON__NONE
            );

            POOL_IS_OPEN = true;
            open_cash();
            set_all_lifeguards();
            sleep(1); // Wait for staff to setup
        }


        // Close pool
        if(POOL_IS_OPEN && TIME_CURR > POOL_CLOSE_TIME){
            log_console(getpid(),
                WHO__POOL_COMPLEX,
                ACTION__CLOSED,
                LOCATION__POOL_COMPLEX,
                REASON__COMPLEX_CLOSED
            );
            close_cash();
            remove_all_clients();
            remove_all_lifeguards();
            POOL_IS_OPEN = false;
        }


        // Clients
        if(TIME_CURR < POOL_OPEN_TIME || TIME_CURR > POOL_CLOSE_TIME){
            log_console(getpid(),
                WHO__POOL_COMPLEX,
                ACTION__CLOSED,
                LOCATION__POOL_COMPLEX,
                REASON__NONE
            );
        }

        else if(rand_client()){
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


        // Time
        sleep(1);
        TIME_CURR += TIME_PER_SEC;
    }
    printf("\n");

    clean_up();
    return 0;
}