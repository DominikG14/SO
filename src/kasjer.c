#include "kasjer.h"


int main(){
    setup();

    MsqBuffer buffer;
    printf_clr(yellow, "%d: kasjer otworzyl kase\n", getpid());
    while(CASH_OPEN){
        // Przyjmij klienta
        buffer.mtype=MSQ_CASH_EMPTY;
        if(msgsnd(CASH_MSQID, &buffer, sizeof(buffer.mvalue), 0) == FAILURE){
            perror("kasjer - msgsnd - klienta");
            exit(EXIT_FAILURE);
        }


        // Przyjmij zaplate
        if(msgrcv(CASH_MSQID, &buffer, sizeof(buffer.mvalue), MSQ_CASH_PAY, 0) == FAILURE){
            if(errno == EINTR){
                continue;
            }
            perror("kasjer - msgrcv - zaplata");
            exit(EXIT_FAILURE);
        }
        printf_clr(yellow, "%d: kasjer otrzymal zaplate\n", getpid());


        // Wrecz paragon
        printf_clr(yellow, "%d: kasjer wreczyl paragon\n", getpid());
        buffer.mtype=MSQ_CASH_BILL;
        if(msgsnd(CASH_MSQID, &buffer, sizeof(buffer.mvalue), 0) == FAILURE){
            perror("kasjer - msgsnd - paragon");
            exit(EXIT_FAILURE);
        }
    }
    printf_clr(yellow, "%d: kasjer zamknal kase\n", getpid());
    printf_clr(yellow, "%d: kasjer opuscil kompleks basenow\n", getpid());

    exit(EXIT_SUCCESS);
}