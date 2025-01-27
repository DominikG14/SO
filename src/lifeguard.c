#include "lifeguard.h"


int main(int argc, char* argv[]){
    GUARDED_POOL = atoi(argv[1]);
    setup();

    log_console(WHO__LIFEGUARD, ACTION__ENTERED, GUARDED_POOL, REASON__NONE);

    while(GUARD){
        // if(rand_int(1, 100) <= LIFEGUARD_CLOSE_POOL_PERC){
        //     log_console(WHO__LIFEGUARD, ACTION__CLOSED, GUARDED_POOL, REASON__NONE);
        //     close_pool();
        //     imitate_time(rand_int(LIFEGUARD_CLOSE_POOL_MIN_TIME, LIFEGUARD_CLOSE_POOL_MAX_TIME));
        //     open_pool();
        //     log_console(WHO__LIFEGUARD, ACTION__OPENED, GUARDED_POOL, REASON__NONE);
        // }
    }

    log_console(WHO__LIFEGUARD, ACTION__LEFT, GUARDED_POOL, REASON__COMPLEX_CLOSED);
    exit(EXIT_SUCCESS);
}