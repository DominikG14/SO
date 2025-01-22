#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "low/signal.h"
#include "low/files.h"
#include "low/key.h"
#include "low/sem.h"
#include "low/msq.h"

#include "logging.h"
#include "random.h"
#include "keys_id.h"


int CASH_MSQID;
int CASH_SEMID;


void close_cash(){
    log_console(getpid(),
        WHO__CASHIER,
        ACTION__CLOSED,
        LOCATION__CASH_QUEUE,
        REASON__COMPLEX_CLOSED
    );

    operate_sem(CASH_SEMID, SEM_CASH_STATUS, SEM_SIGNAL);
    delete_msq(CASH_MSQID);
}


void open_cash(){
    key_t key = get_key(CASH_KEY_ID);
    CASH_SEMID = access_sem(key, SEM_CASH_NUM, IPC_CREAT|0600);
    init_sem(CASH_SEMID, 2);
    CASH_MSQID = access_msq(key, IPC_CREAT|0600);

    handle_signal(SIG_CLOSE_POOL, close_cash);
    
    log_console(getpid(),
        WHO__CASHIER,
        ACTION__OPENED,
        LOCATION__CASH_QUEUE,
        REASON__NONE
    );
}


int accept_payment(){
    int status;
    int return_msqid;
    char* client_pid_str = NULL;
    int fd_fifo;

    status = USget_msq(CASH_MSQID, 1, &client_pid_str);

    if(status != MSQ_FAILURE){
        log_console(getpid(),
            WHO__CASHIER,
            ACTION__RECIVED_PAY,
            LOCATION__CASH_QUEUE,
            REASON__NONE
        );

        sleep(1);
        fd_fifo = open(client_pid_str, O_WRONLY);

        if(fd_fifo == FILE_FAILURE){
            perror(__func__);
            exit(EXIT_FAILURE);
        }
    }
    write(fd_fifo, "OK", 3);

    free(client_pid_str), client_pid_str = NULL;
    return status;
}


void next_client(){
    operate_sem(CASH_SEMID, SEM_CASH_PAYMENT, SEM_SIGNAL);
}


int cash_closed(){
    return USget_sem_value(CASH_SEMID, SEM_CASH_STATUS);
}


int main(){
    open_cash();

    int payment_time;
    int payment_status;
    while(true){
        payment_time = rand_int(1, 3);
        sleep(payment_time);

        next_client();
        if(cash_closed()) break;

        payment_status = accept_payment();
        if(payment_status != FAILURE) continue;

        perror(__func__);
        exit(EXIT_FAILURE);
    }

    delete_sem(CASH_SEMID);
    log_console(getpid(),
        WHO__CASHIER,
        ACTION__LEFT,
        LOCATION__POOL_COMPLEX,
        REASON__COMPLEX_CLOSED
    );
    return 0;
}