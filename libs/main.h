#pragma once

#include "colors.h"
#include "global.h"
#include "low.h"
#include "config.h"
#include "random.h"
#include "logging.h"


// -------------------- Local --------------------
int TIME_CURR;

int PID_CASHIER;

int PID_CLIENTS[1440];
int CLIENTS_NUM = 0;

bool COMPLEX_IS_OPEN = false;


// -------------------- Bools --------------------
bool open_complex_time(){
    return !COMPLEX_IS_OPEN && TIME_CURR >= COMPLEX_OPEN_TIME && TIME_CURR <= COMPLEX_CLOSE_TIME;
}


bool close_complex_time(){
    return COMPLEX_IS_OPEN && TIME_CURR > COMPLEX_CLOSE_TIME;
}


bool rand_client(){
    int spawn_client_perc = rand_int(0, 100);
    if(spawn_client_perc <= CLIENT_SPAWN_PERC){
        return true;
    }

    return false;
}


// -------------------- Time --------------------
// Display time in HH:MM format
void disp_time(int time){
    int hour = time / 60;
    int min = time % 60;

    bold_underline_cyan();
    printf("\n");
    if(hour < 10){
        printf("0");
    }
    printf("%d:", hour);

    if(min < 10){
        printf("0");
    }
    printf("%d", min);
    reset();
    printf("\n");
}


void disp_curr_time(){
    if(open_complex_time()) disp_time(COMPLEX_OPEN_TIME);
    else if(close_complex_time()) disp_time(COMPLEX_CLOSE_TIME);
    else disp_time(TIME_CURR);
}


// -------------------- Pool Complex functionality --------------------
void open_pools(){
    MSQ_BUFFER.mtype=MSQ_POOL_SPACE;


    // Olimpic pool
            // Each client comes alone
    for(int i = 0; i < POOL_OLIMPIC_MAX_SIZE; i++){
        if(msgsnd(OLIMPIC_POOL_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), 0) == FAILURE){
            perror("main - msgsnd - open_pool");
            exit(EXIT_FAILURE);
        }
    }
    semctl(OLIMPIC_POOL_SEMID, SEM_POOL_SHM, SETVAL, SEM_SIGNAL);


    // Leisure pool
            // Clients come with childs or alone
    SEM_OPERATE.sem_op = SEM_SIGNAL;
    semop(LEISURE_POOL_SEMID, &SEM_OPERATE, 1);


    // Paddling pool
            // Each client comes with a child
    for(int i = 0; i < POOL_PADDLING_MAX_SIZE / 2; i++){
        if(msgsnd(PADDLING_POOL_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), 0) == FAILURE){
            perror("main - msgsnd - open_pool");
            exit(EXIT_FAILURE);
        }
    }
    semctl(PADDLING_POOL_SEMID, SEM_POOL_SHM, SETVAL, SEM_SIGNAL);
}


void let_clients_in(){
    int pid;

    if(!COMPLEX_IS_OPEN){
        log_console(WHO__POOL_COMPLEX, ACTION__CLOSED, LOCATION__POOL_COMPLEX, REASON__NONE);
        return;
    }

    if(rand_client()){
    switch(pid = fork()){
        case FAILURE:
            perror("main - fork");
            exit(EXIT_FAILURE);

        case SUCCESS:
            execl(PS_CLIENT_PATH, PS_CLIENT_NAME, NULL);
            perror("main - execl");
            exit(EXIT_FAILURE);
        
        default:
            PID_CLIENTS[CLIENTS_NUM] = pid;
            CLIENTS_NUM++;
        }
    }
}


void remove_all_clients(){
    for(int i = 0; i < CLIENTS_NUM; i++){
        kill(PID_CLIENTS[i], SIG_CLOSE_COMPLEX);
        waitpid(PID_CLIENTS[i], NULL, 0);
    }
}


void open_cash(){
    switch(PID_CASHIER = fork()){
        case FAILURE:
            perror("main - fork");
            exit(EXIT_FAILURE);

        case SUCCESS:
            execl(PS_CASHIER_PATH, PS_CASHIER_NAME, NULL);
            perror("main - execl");
            exit(EXIT_FAILURE);
    }
}


void close_cash(){
    kill(PID_CASHIER, SIG_CLOSE_COMPLEX);
    waitpid(PID_CASHIER, NULL, 0);
}


void open_complex(){
    log_console(WHO__POOL_COMPLEX, ACTION__OPENED, LOCATION__POOL_COMPLEX, REASON__NONE);
    COMPLEX_IS_OPEN = true;
    open_cash();
    open_pools();
}


void close_complex(){
    log_console(WHO__POOL_COMPLEX, ACTION__CLOSED, LOCATION__POOL_COMPLEX, REASON__COMPLEX_CLOSED);

    close_cash();
    remove_all_clients();
}


// -------------------- IPCS --------------------
void clean_up(){
    // Cash
    msgctl(CASH_MSQID, IPC_RMID, NULL);

    // Olimpic pool
    msgctl(OLIMPIC_POOL_MSQID, IPC_RMID, NULL);
    semctl(OLIMPIC_POOL_SEMID, 0, IPC_RMID, NULL);
    shmctl(OLIMPIC_POOL_SHMID, IPC_RMID, NULL);

    // Leisure pool
    msgctl(LEISURE_POOL_MSQID, IPC_RMID, NULL);
    semctl(LEISURE_POOL_SEMID, 0, IPC_RMID, NULL);
    shmctl(LEISURE_POOL_SHMID, IPC_RMID, NULL);

    // Paddling pool
    msgctl(PADDLING_POOL_MSQID, IPC_RMID, NULL);
    semctl(PADDLING_POOL_SEMID, 0, IPC_RMID, NULL);
    shmctl(PADDLING_POOL_SHMID, IPC_RMID, NULL);
}


void __shutdown_handler(int sig){
    clean_up();
    printf_clr(red, "SHUTDOWN!");
    exit(EXIT_FAILURE);
}


void __set_shutdown_handler(){
    struct sigaction handler;
    handler.sa_handler = __shutdown_handler;
    sigemptyset(&handler.sa_mask);
    handler.sa_flags = 0;
    sigaction(SIGINT, &handler, 0);
}


void __reset_logs(){
    if(remove(LOGGING_FILEPATH) != 0);
}


void __create_cash_msq(){
    key_t key = get_key(KEY_CASH_MSQ);
    if((CASH_MSQID = msgget(key, IPC_CREAT|0600)) == FAILURE){
        perror("main - msgget - kasa");
        exit(EXIT_FAILURE);
    }
}


void __create_pools(){
    key_t key_msq, key_sem, key_shm;
    int* pool;

    // Olimpic pool
    key_msq = get_key(KEY_OLIMPIC_POOL_MSQ);
    key_sem = get_key(KEY_OLIMPIC_POOL_SEM);
    key_shm = get_key(KEY_OLIMPIC_POOL_SHM);

    if((OLIMPIC_POOL_MSQID = msgget(key_msq, IPC_CREAT|0600)) == FAILURE){
        perror("main - msgget - pools");
        exit(EXIT_FAILURE);
    }

    if((OLIMPIC_POOL_SEMID = semget(key_sem, SEM_POOL_NUM, IPC_CREAT|0600)) == FAILURE){
        perror("main - semget - pools");
        exit(EXIT_FAILURE);
    }

    if((OLIMPIC_POOL_SHMID = shmget(key_shm, sizeof(int), IPC_CREAT|0600)) == FAILURE){
        perror("main - shmget - pools");
        exit(EXIT_FAILURE);
    }
    pool =(int*) shmat(OLIMPIC_POOL_SHMID, NULL, 0);
    pool = 0;
    shmdt(pool);


    // Leisure pool
    key_msq = get_key(KEY_LEISURE_POOL_MSQ);
    key_sem = get_key(KEY_LEISURE_POOL_SEM);
    key_shm = get_key(KEY_LEISURE_POOL_SHM);

    if((LEISURE_POOL_MSQID = msgget(key_msq, IPC_CREAT|0600)) == FAILURE){
        perror("main - msgget - pools");
        exit(EXIT_FAILURE);
    }

    if((LEISURE_POOL_SEMID = semget(key_sem, SEM_POOL_NUM, IPC_CREAT|0600)) == FAILURE){
        perror("main - semget - pools");
        exit(EXIT_FAILURE);
    }

        // For size and age_sum
    if((LEISURE_POOL_SHMID = shmget(key_shm, 2*sizeof(int), IPC_CREAT|0600)) == FAILURE){
        perror("main - shmget - pools");
        exit(EXIT_FAILURE);
    }
    pool =(int*) shmat(LEISURE_POOL_SHMID, NULL, 0);
    pool[LEISURE_POOL_SIZE] = 0;
    pool[LEISURE_POOL_AGE_SUM] = 0;
    shmdt(pool);


    // Paddling pool
    key_msq = get_key(KEY_PADDLING_POOL_MSQ);
    key_sem = get_key(KEY_PADDLING_POOL_SEM);
    key_shm = get_key(KEY_PADDLING_POOL_SHM);

    if((PADDLING_POOL_MSQID = msgget(key_msq, IPC_CREAT|0600)) == FAILURE){
        perror("main - msgget - pools");
        exit(EXIT_FAILURE);
    }

    if((PADDLING_POOL_SEMID = semget(key_sem, SEM_POOL_NUM, IPC_CREAT|0600)) == FAILURE){
        perror("main - semget - pools");
        exit(EXIT_FAILURE);
    }

    if((PADDLING_POOL_SHMID = shmget(key_shm, sizeof(int), IPC_CREAT|0600)) == FAILURE){
        perror("main - shmget - pools");
        exit(EXIT_FAILURE);
    }
    pool =(int*) shmat(PADDLING_POOL_SHMID, NULL, 0);
    pool = 0;
    shmdt(pool);
}


void setup(){
    __set_shutdown_handler();
    set_config_vars();
    check_config();
    __reset_logs();
    __create_cash_msq();
    __create_pools();
}