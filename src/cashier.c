#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "low/signal.h"
#include "low/files.h"
#include "low/key.h"
#include "low/sem.h"
#include "low/msg_q.h"

#include "random.h"
#include "keys_id.h"


int CASHIER__MSQID;
int CASHIER__SEMID;


void open_cash(){
    key_t key = get_key(CASHIER__KEY_ID);

    CASHIER__MSQID = access_msg_q(key, IPC_CREAT|0600);

    CASHIER__SEMID = access_sem(key, 2, IPC_CREAT|0600);
    init_sem(CASHIER__SEMID, 2);
    operate_sem(CASHIER__SEMID, SEM__CASHIER_PAYMENT, SEM__SIGNAL);

    printf("%d: Cash opened\n", getpid());
}


void close_cash(){
    operate_sem(CASHIER__SEMID, SEM__CASHIER_STATUS, SEM__SIGNAL); // close cash

    // Signaling Clients to leave
    while(get_sem_value(CASHIER__SEMID, SEM__CASHIER_PAYMENT) < 0) {
        operate_sem(CASHIER__SEMID, SEM__CASHIER_PAYMENT, SEM__SIGNAL);
    }

    // Discarding messages
    MSG_Q__BUFFER msg;
    while(msgrcv(CASHIER__MSQID, &msg, sizeof(msg.text), 0, IPC_NOWAIT) != -1) {
        printf("Discarding message\n");
    }

    delete_sem(CASHIER__SEMID);
    delete_msg_q(CASHIER__MSQID);
}


int main(){
    open_cash();

    handle_signal(SIG__CLOSE_POOL, close_cash);
    operate_sem(CASHIER__SEMID, SEM__CASHIER_PAYMENT, SEM__SIGNAL);

    char* text;
    int payment_time;
    while(get_sem_value(CASHIER__SEMID, SEM__CASHIER_STATUS) == 0){
        payment_time = 10;
        sleep(payment_time);
        text = get_msg_q(CASHIER__MSQID, 1);
        printf("\t%s\n", text);
        free(text), text = NULL;

        operate_sem(CASHIER__SEMID, SEM__CASHIER_PAYMENT, SEM__SIGNAL);
    }

    printf("%d: Cash closed\n", getpid());
    
    return 0;
}