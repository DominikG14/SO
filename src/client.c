#include "client.h"


int main(){
    setup();
    MsqBuffer buffer;


    // Wejdz do kolejki
    CLIENT_LOCATION = LOCATION_CASH_QUEUE;
    if(msgrcv(CASH_MSQID, &buffer, sizeof(buffer.mvalue), MSQ_CASH_EMPTY, 0) == FAILURE && errno != EINTR){
        perror("klient - msgrcv");
        exit(EXIT_FAILURE);
    }


    // Wrecz zaplate
    printf_clr(blue, "%d: klient zaplacil\n", getpid());
    buffer.mtype=MSQ_CASH_PAY;
    if(msgsnd(CASH_MSQID, &buffer, sizeof(buffer.mvalue), 0) == FAILURE){
        perror("klient - msgsnd");
        exit(EXIT_FAILURE);
    }


    // Czekaj na paragon
    if(msgrcv(CASH_MSQID, &buffer, sizeof(buffer.mvalue), MSQ_CASH_BILL, 0) == FAILURE && errno != EINTR){
        perror("klient - msgrcv");
        exit(EXIT_FAILURE);
    }
    printf_clr(blue, "%d: klient opuscil kolejke\n", getpid());
    if(client_has_child()){
        child.tid = new_thread(child_leave_cash_queue, NULL);
        pthread_join(child.tid, NULL);
    }


    // Wejdz do basenu
    client_choose_pool();

    while(true); // plywa w basenie
}