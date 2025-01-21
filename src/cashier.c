#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "low/signal.h"
#include "low/files.h"
#include "low/key.h"
#include "low/sem.h"
#include "low/msg_q.h"

#include "keys_id.h"


bool CASHIER__CASH_OPEN = true;
char* CASHIER__TMP_FILE_MSQID = "./tmp/cash_msqid";
char* CASHIER__TMP_FILE_SEMID = "./tmp/cash_semid";


int CASHIER__MSQID;
int CASHIER__SEMID;


void open_cash(){
    key_t key = get_key(CASHIER__KEY_ID);

    CASHIER__MSQID = access_msg_q(key, IPC_CREAT|0600);
    save_id(CASHIER__TMP_FILE_MSQID, CASHIER__MSQID);

    CASHIER__SEMID = access_sem(key, 1, IPC_CREAT|0200);
    save_id(CASHIER__TMP_FILE_SEMID, CASHIER__SEMID);
    operate_sem(CASHIER__SEMID, 0, SEM__SIGNAL);

    printf("%d: Cash opened\n", getpid());
}


void close_cash(){
    CASHIER__CASH_OPEN = false;
    delete_sem(CASHIER__SEMID);
    delete_msg_q(CASHIER__MSQID);

    printf("%d: Cash closed\n", getpid());
}


int main(){
    open_cash();

    handle_signal(SIG__CLOSE_POOL, close_cash);
    while(CASHIER__CASH_OPEN){
        // TODO: Add cash functionality here
    }

    return 0;
}