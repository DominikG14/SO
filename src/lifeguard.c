#include "lifeguard.h"


int main(int argc, char* argv[]){
    GUARDED_POOL = atoi(argv[1]);
    setup();


    // Tell the pool that lifeguard is set
    key_t key = get_key(KEY_CASH_MSQ);
    CASH_MSQID = msgget(key, 0600);

    MSQ_BUFFER.mtype=MSQ_COMPLEX_OPEN;
    if(msgsnd(CASH_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), 0) == FAILURE){
        perror("lifeguard - msgsnd - open_pool");
        exit(EXIT_FAILURE);
    }

    SEM_OPERATE.sem_num=SEM_POOL_SHM;
    SEM_OPERATE.sem_op=SEM_WAIT;
    semop(POOL_SEMID, &SEM_OPERATE, 1);

    log_console(WHO__LIFEGUARD, ACTION__ENTERED, GUARDED_POOL, REASON__NONE);


    // Guard the pool
    while(true){
        if(rand_int(1, 100) <= LIFEGUARD_CLOSE_POOL_PERC){
            close_pool();
        }
        imitate_time(1);
    }
}