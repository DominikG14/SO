#include "client.h"


int main(){
    setup();

    // Wejdz do kolejki
    set_client_info(ACTION__ENTERED, LOCATION__CASH_QUEUE, REASON__NONE), log_client(WHO__CLIENT);
    if(client_has_child()){
        child.tid = new_thread(child_keep_eye_on, NULL);
        pthread_join(child.tid, NULL);
    }
    if(msgrcv(CASH_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), MSQ_CASH_EMPTY, 0) == FAILURE && errno != EINTR){
        perror("klient - msgrcv");
        exit(EXIT_FAILURE);
    }


    // Wrecz zaplate
    set_client_info(ACTION__PAID, LOCATION__CASH_QUEUE, REASON__NONE), log_client(WHO__CLIENT);
    MSQ_BUFFER.mtype=MSQ_CASH_PAY;
    if(msgsnd(CASH_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), 0) == FAILURE){
        perror("klient - msgsnd");
        exit(EXIT_FAILURE);
    }


    // Czekaj na paragon
    if(msgrcv(CASH_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), MSQ_CASH_BILL, 0) == FAILURE && errno != EINTR){
        perror("klient - msgrcv");
        exit(EXIT_FAILURE);
    }
    set_client_info(ACTION__RECIVED_BILL, LOCATION__CASH_QUEUE, REASON__NONE), log_client(WHO__CLIENT);
    set_client_info(ACTION__LEFT, LOCATION__CASH_QUEUE, REASON__PAYMENT_FINISHED), log_client(WHO__CLIENT);
    if(client_has_child()){
        child.tid = new_thread(child_keep_eye_on, NULL);
        pthread_join(child.tid, NULL);
    }


    // Wejdz do basenu
    client_choose_pool();

    while(true); // plywa w basenie
}