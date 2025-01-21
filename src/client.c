#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "client.h"
#include "low/signal.h"
#include "low/sem.h"
#include "low/files.h"
#include "low/msg_q.h"
#include "keys_id.h"


bool CLIENT__SWIM = true;

void leave_pool(){
    CLIENT__SWIM = false;
}


int main(){
    setup();

    Client client;

    client.age = rand_int(11, 70);
    client.swim_cap_on = rand_swim_cap();
    int child_age = rand_int(1, 10);

    printf("%d: swim_cap: %d, age %d", getpid(), client.swim_cap_on, client.age);
    if(rand_child(client.age, child_age)){
        printf(", child: %d", child_age);
    }
    printf("\n");

    handle_signal(SIG__CLOSE_POOL, leave_pool);
    int cash_semid = get_id(CASHIER__TMP_FILE_SEMID);
    int cash_msqid = get_id(CASHIER__TMP_FILE_MSQID);

    operate_sem(cash_semid, 0, SEM__WAIT);
    send_msg_q(cash_msqid, "TEST", 1);

    while(CLIENT__SWIM){

    }
    printf("%d: Client left the pool\n", getpid());

    return 0;
}