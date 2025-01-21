#include <stdbool.h>
#include <stdio.h>

#include "low/signal.h"


bool CASH_OPEN = true;


void close_cash(){
    CASH_OPEN = false;
}


int main(int argc, char* argv[]){
    int pool_num = atoi(argv[1]);
    printf("%d: Cash opened for pool: %d\n", getpid(), pool_num);

    handle_signal(SIGUSR1, close_cash);
    while(CASH_OPEN){
        // TODO: Add cash functionality here
    }

    printf("%d: Cash closed for pool: %d\n", getpid(), pool_num);

    return 0;
}