#include "lifeguard.h"


int main(int argc, char* argv[]){
    GUARDED_POOL = atoi(argv[1]);
    setup();

    LOG_enter_guarder_pool();

    while(true){
        if(rand_int(1, 100) <= LIFEGUARD_CLOSE_POOL_PERC){
            send_signal(0, SIG_LIFEGUARD_CLOSE_POOL);

            sleep(rand_int(LIFEGUARD_CLOSE_POOL_MIN_TIME, LIFEGUARD_CLOSE_POOL_MAX_TIME));
        }
    }
    return 0;
}