#include "klient.h"


int main(){
    setup();
    MsqBuffer buffer;


    // Wejdz do kolejki
    OBECNA_LOKACJA = LOCATION_CASH_QUEUE;
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


    // Wejdz do basenu
    switch(rand_int(0, 2)){
        case OLIMPIC:
            OBECNA_LOKACJA = LOCATION_OLIMPIC_POOL;
            printf_clr(blue, "%d: klient wszedl do basenu olimpijskiego\n", getpid());
            break;
        
        case LEISURE:
            OBECNA_LOKACJA = LOCATION_LEISURE_POOL;
            printf_clr(blue, "%d: klient wszedl do basenu rekreacyjnego\n", getpid());
            break;

        case PADDLING:
            OBECNA_LOKACJA = LOCATION_PADDLING_POOL;
            printf_clr(blue, "%d: klient wszedl do brodziku\n", getpid());
            break;
    }

    while(true); // plywa w basenie
}