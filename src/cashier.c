#include "cashier.h"


int main(){
    setup();
    log_console(WHO__CASHIER, ACTION__OPENED, LOCATION__CASH_QUEUE, REASON__NONE);


    while(CASH_OPEN){

        // Get client
        MSQ_BUFFER.mtype=MSQ_CASH_EMPTY;
        if(msgsnd(CASH_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), 0) == FAILURE){
            perror("kasjer - msgsnd - klienta");
            exit(EXIT_FAILURE);
        }


        // Get Payment
        if(msgrcv(CASH_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), MSQ_CASH_PAY, 0) == FAILURE){
            perror("kasjer - msgrcv - zaplata");
            exit(EXIT_FAILURE);
        }
        log_console(WHO__CASHIER, ACTION__RECIVED_PAY, LOCATION__CASH_QUEUE, REASON__NONE);


        imitate_time(rand_int(CASHIER_MIN_PAYMENT_TIME, CASHIER_MAX_PAYMENT_TIME));
        

        // Give bill
        MSQ_BUFFER.mtype=MSQ_CASH_BILL;
        if(msgsnd(CASH_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), 0) == FAILURE){
            perror("kasjer - msgsnd - paragon");
            exit(EXIT_FAILURE);
        }
    }


    log_console(WHO__CASHIER, ACTION__CLOSED, LOCATION__CASH_QUEUE, REASON__COMPLEX_CLOSED);
    exit(EXIT_SUCCESS);
}