#include "cashier.h"


int main(){
    setup();
    log_console(WHO__CASHIER, ACTION__OPENED, LOCATION__CASH_QUEUE, REASON__NONE);


    while(CASH_OPEN){
        // Get client
        send_msq(CASH_MSQID, MSQ_CASH_EMPTY);

        // Get Payment
        get_msq(CASH_MSQID, MSQ_CASH_PAY);
        log_console(WHO__CASHIER, ACTION__RECIVED_PAY, LOCATION__CASH_QUEUE, REASON__NONE);

        // Realize payment
        imitate_time(rand_int(CASHIER_MIN_PAYMENT_TIME, CASHIER_MAX_PAYMENT_TIME));
        
        // Give bill
        send_msq(CASH_MSQID, MSQ_CASH_BILL);
    }


    log_console(WHO__CASHIER, ACTION__CLOSED, LOCATION__CASH_QUEUE, REASON__COMPLEX_CLOSED);
    exit(EXIT_SUCCESS);
}