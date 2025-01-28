#include "cashier.h"


int main(){
    setup();

    // Tell the pool that cash opened
    MSQ_BUFFER.mtype=MSQ_COMPLEX_OPEN;
    if(msgsnd(CASH_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), 0) == FAILURE){
        perror("cashier - msgsnd - open_pool");
        exit(EXIT_FAILURE);
    }

    key_t key = get_key(KEY_OLIMPIC_POOL_SEM);
    POOL_SEMID = semget(key, SEM_POOL_NUM, 0200);

    SEM_OPERATE.sem_num=SEM_POOL_SHM;
    SEM_OPERATE.sem_op=SEM_WAIT;
    semop(POOL_SEMID, &SEM_OPERATE, 1);

    log_console(WHO__CASHIER, ACTION__OPENED, LOCATION__CASH_QUEUE, REASON__NONE);


    while(CASH_OPEN){

        // Przyjmij Client
        MSQ_BUFFER.mtype=MSQ_CASH_EMPTY;
        if(msgsnd(CASH_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), 0) == FAILURE){
            perror("cashier - msgsnd - client");
            exit(EXIT_FAILURE);
        }


        // Payment
        if(msgrcv(CASH_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), MSQ_CASH_PAY, 0) == FAILURE){
            if(errno == EINTR) continue;
            perror("cashier - msgrcv - payment");
            exit(EXIT_FAILURE);
        }
        log_console(WHO__CASHIER, ACTION__RECIVED_PAY, LOCATION__CASH_QUEUE, REASON__NONE);


        // Bill
        MSQ_BUFFER.mtype=MSQ_CASH_BILL;
        if(msgsnd(CASH_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), 0) == FAILURE){
            perror("cashier - msgsnd - bill");
            exit(EXIT_FAILURE);
        }
    }


    log_console(WHO__CASHIER, ACTION__CLOSED, LOCATION__CASH_QUEUE, REASON__COMPLEX_CLOSED);
    exit(EXIT_SUCCESS);
}