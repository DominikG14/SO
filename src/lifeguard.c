#include "lifeguard.h"


int main(int argc, char* argv[]){
    GUARDED_POOL = atoi(argv[1]);
    setup();

    LOG_enter_guarder_pool();

    while(true){
        sleep(1);
        if(rand_int(1, 100) <= LIFEGUARD_CLOSE_POOL_PERC){
            LOG_closed_pool();
            close_pool();
            sleep(1 + rand_int(LIFEGUARD_CLOSE_POOL_MIN_TIME, LIFEGUARD_CLOSE_POOL_MAX_TIME));
            LOG_pool_reopened();
            open_pool();
        }
    }
}