#include "kasjer.h"


int main(){
    setup();

    MsqBuffer buffer;
    printf_clr(yellow, "%d: kasjer otworzyl kase\n", getpid());
    while(KASA_OTWARTA){
        // Przyjmij klienta
        buffer.mtype=MSQ_KASA_WOLNA;
        if(msgsnd(KASA_MSQID, &buffer, sizeof(buffer.mvalue), 0) == FAILURE){
            perror("kasjer - msgsnd - klienta");
            exit(EXIT_FAILURE);
        }


        // Przyjmij zaplate
        if(msgrcv(KASA_MSQID, &buffer, sizeof(buffer.mvalue), MSQ_KASA_ZAPLATA, 0) == FAILURE && errno != EINTR){
            if(errno == EINTR){
                continue;
            }
            perror("kasjer - msgrcv - zaplata");
            exit(EXIT_FAILURE);
        }
        printf_clr(yellow, "%d: kasjer otrzymal zaplate\n", getpid());


        // Wrecz paragon
        printf_clr(yellow, "%d: kasjer wreczyl paragon\n", getpid());
        buffer.mtype=MSQ_KASA_RACHUNEK;
        if(msgsnd(KASA_MSQID, &buffer, sizeof(buffer.mvalue), 0) == FAILURE && errno != EINTR){
            perror("kasjer - msgsnd - paragon");
            exit(EXIT_FAILURE);
        }
    }
    printf_clr(yellow, "%d: kasjer zamknal kase\n", getpid());
    printf_clr(yellow, "%d: kasjer opuscil kompleks basenow\n", getpid());

    return 0;
}