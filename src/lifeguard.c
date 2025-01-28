#include "lifeguard.h"


int main(int argc, char* argv[]){
    GUARDED_POOL = atoi(argv[1]);
    setup();

    log_console(WHO__LIFEGUARD, ACTION__ENTERED, GUARDED_POOL, REASON__NONE);

    while(true){
        if(rand_int(1, 100) <= LIFEGUARD_CLOSE_POOL_PERC){
            close_pool();
        }
        imitate_time(1);
    }
}