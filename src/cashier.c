#include <stdbool.h>

#include "low/signal.h"


bool CASH_OPEN = true;


void close_cash(){
    CASH_OPEN = false;
}


int main(int argc, char *argv[]){
    int pool_num = argv[1] - '0';
    printf("%d: cash opened for pool: %d", getpid(), pool_num);

    handle_signal(SIGUSR1, close_cash);
    while(CASH_OPEN){
        // TODO: Add cash functionality here
    }

    printf("%d: cash closed for pool: %d", getpid(), pool_num);

    return 0;
}