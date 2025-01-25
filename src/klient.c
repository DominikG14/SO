#include "klient.h"


int main(){
    setup();
    MsqBuffer buffer;



    // Wejdz do kolejki
    printf_clr(blue, "%d: klient wszedl do kolejki\n", getpid());
    OBECNA_LOKACJA = LOKACJA_KASA;
    if(msgrcv(KASA_MSQID, &buffer, sizeof(buffer.mvalue), MSQ_KASA_WOLNA, 0) == FAILURE && errno != EINTR){
        perror("klient - msgrcv");
        exit(EXIT_FAILURE);
    }


    // Wrecz zaplate
    printf_clr(blue, "%d: klient zaplacil\n", getpid());
    buffer.mtype=MSQ_KASA_ZAPLATA;
    if(msgsnd(KASA_MSQID, &buffer, sizeof(buffer.mvalue), 0) == FAILURE){
        perror("klient - msgsnd");
        exit(EXIT_FAILURE);
    }


    // Czekaj na paragon
    if(msgrcv(KASA_MSQID, &buffer, sizeof(buffer.mvalue), MSQ_KASA_RACHUNEK, 0) == FAILURE && errno != EINTR){
        perror("klient - msgrcv");
        exit(EXIT_FAILURE);
    }
    printf_clr(blue, "%d: klient opuscil kolejke\n", getpid());


    OBECNA_LOKACJA = LOKACJA_BASEN;
    switch(rand_int(1, 3)){

    }


    return 0;
}