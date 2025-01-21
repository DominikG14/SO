#include <stdbool.h>
#include <stdio.h>

#include "low/signal.h"


bool LIFEGUARD__WATCH_POOL = true;


void remove_lifeguard(){
    LIFEGUARD__WATCH_POOL = false;
}


int main(int argc, char* argv[]){
    int pool_num = atoi(argv[1]);
    printf("%d: Lifeguard set for pool: %d\n", getpid(), pool_num);

    handle_signal(SIGUSR1, remove_lifeguard);
    while(LIFEGUARD__WATCH_POOL){
        // TODO: Add lifeguard functionality here
    }

    printf("%d: Lifeguard left pool: %d\n", getpid(), pool_num);

    return 0;
}