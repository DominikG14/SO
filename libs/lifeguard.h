#include "config.h"
#include "logging.h"
#include "global.h"
#include "random.h"


// -------------------- Local --------------------
bool GUARD = true;
bool CLOSED_POOL_ONCE = false;
int GUARDED_POOL;
int POOL_MAX_SIZE;


// -------------------- Lifeguard functionality --------------------
void close_pool(){
    PoolData* pool =(PoolData*) shmat(POOL_SHMID, NULL, 0);

    // Block pool access until open again
    SEM_OPERATE.sem_flg = 0;
    SEM_OPERATE.sem_num = SEM_POOL_SHM;
    SEM_OPERATE.sem_op = SEM_WAIT;
    semop(POOL_SEMID, &SEM_OPERATE, 1);
    POOL_SIZE = pool->size;
    log_console(WHO__LIFEGUARD, ACTION__CLOSED, GUARDED_POOL, REASON__NONE);
    

    SEM_OPERATE.sem_num = SEM_POOL_LIFEGUARD;
    SEM_OPERATE.sem_op = SEM_SIGNAL;
    semop(POOL_SEMID, &SEM_OPERATE, 1);

    // Remove clients
    for(int i = 0; i < MAX_CLIENTS_PER_DAY; i++){
        if(pool->pid_clients[i] == -1) continue;
        kill(pool->pid_clients[i], SIG_LIFEGUARD);
    }

    // Wait for clients to leave the pool
    int clients_left = 0;
    while(clients_left != POOL_SIZE){
        get_msq(POOL_MSQID, MSQ_LIFEGUARD);
        clients_left++;
    }

    // Keep pool closed
    imitate_time(rand_int(LIFEGUARD_CLOSE_POOL_MIN_TIME, LIFEGUARD_CLOSE_POOL_MAX_TIME));

    // Open pool
    int already_waiting = get_msq_count(POOL_MSQID);
    for(int i = already_waiting; i < POOL_MAX_SIZE; i++){
        send_msq(POOL_MSQID, MSQ_POOL_SPACE);
    }

    log_console(WHO__LIFEGUARD, ACTION__OPENED, GUARDED_POOL, REASON__NONE);
    SEM_OPERATE.sem_num = SEM_POOL_SHM;
    SEM_OPERATE.sem_op = SEM_SIGNAL;
    semop(POOL_SEMID, &SEM_OPERATE, 1);

    CLOSED_POOL_ONCE = true;
}


// -------------------- IPCS --------------------
void __close_complex_handler(){
    GUARD = false;
}


void __access_pool(){
    SEM_OPERATE.sem_flg = 0;

    key_t key_msq, key_sem, key_shm;

    switch(GUARDED_POOL){
        case OLIMPIC:
            key_msq = get_key(KEY_OLIMPIC_POOL_MSQ);
            key_sem = get_key(KEY_OLIMPIC_POOL_SEM);
            key_shm = get_key(KEY_OLIMPIC_POOL_SHM);
            POOL_MAX_SIZE = POOL_OLIMPIC_MAX_SIZE;
        break;

        case LEISURE:
            key_msq = get_key(KEY_LEISURE_POOL_MSQ);
            key_sem = get_key(KEY_LEISURE_POOL_SEM);
            key_shm = get_key(KEY_LEISURE_POOL_SHM);
            POOL_MAX_SIZE = POOL_LEISURE_MAX_SIZE;
        break;

        case PADDLING:
            key_msq = get_key(KEY_PADDLING_POOL_MSQ);
            key_sem = get_key(KEY_PADDLING_POOL_SEM);
            key_shm = get_key(KEY_PADDLING_POOL_SHM);
            POOL_MAX_SIZE = POOL_PADDLING_MAX_SIZE / 2;
        break;
    }


    if((POOL_MSQID = msgget(key_msq, 0600)) == FAILURE){
        perror("lifeguard - msgget");
        exit(EXIT_FAILURE);
    }

    if((POOL_SEMID = semget(key_sem, SEM_POOL_NUM, 0600)) == FAILURE){
        perror("lifeguard - semget");
        exit(EXIT_FAILURE);
    }

    if((POOL_SHMID = shmget(key_shm, sizeof(PoolData), 0600)) == FAILURE){
        perror("lifeguard - shmget");
        exit(EXIT_FAILURE);
    }
}


void __set_close_complex_handler(){
    struct sigaction handler;
    handler.sa_handler = __close_complex_handler;
    sigemptyset(&handler.sa_mask);
    handler.sa_flags = 0;
    sigaction(SIG_CLOSE_COMPLEX, &handler, 0);
}


void setup(){
    srand(getpid());
    __set_close_complex_handler();
    set_config_vars();
    __access_pool();
}