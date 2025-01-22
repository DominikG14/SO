#include <stdbool.h>
#include <stdio.h>

#include "low/signal.h"
#include "logging.h"


bool LIFEGUARD_WATCH_POOL = true;
int GUARDED_POOL;


void remove_lifeguard(){
    LIFEGUARD_WATCH_POOL = false;
}


int main(int argc, char* argv[]){
    GUARDED_POOL = atoi(argv[1]);

    log_console(getpid(),
        WHO__LIFEGUARD,
        ACTION__ENTERED,
        GUARDED_POOL,
        REASON__NONE
    );

    handle_signal(SIGUSR1, remove_lifeguard);
    while(LIFEGUARD_WATCH_POOL){
        // TODO: Add lifeguard functionality here
    }

    log_console(getpid(),
        WHO__LIFEGUARD,
        ACTION__LEFT,
        GUARDED_POOL,
        REASON__COMPLEX_CLOSED
    );

    return 0;
}