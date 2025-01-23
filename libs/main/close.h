#pragma once

#include "vars.h"
#include "low.h"
#include "logging.h"
#include "pool.h"


void close_cash(){
    send_signal(CASHIER_PID, SIG_CLOSE_POOL);
    waitpid(CASHIER_PID, NULL, 0);
}


void remove_all_lifeguards(){
    for(int pool_num = 0; pool_num < POOL_NUM; pool_num++){
        send_signal(LIFEGUARD_PIDS[pool_num], SIG_CLOSE_POOL);
        waitpid(LIFEGUARD_PIDS[pool_num], NULL, 0);
    }
}


void remove_all_clients(){
    for(int i = 0; i < CLIENTS_NUM; i++){
        send_signal(CLIENT_PIDS[i], SIG_CLOSE_POOL);
        waitpid(CLIENT_PIDS[i], NULL, 0);
    }
}


void close_complex(){
    log_console(getpid(),
        WHO__POOL_COMPLEX,
        ACTION__CLOSED,
        LOCATION__POOL_COMPLEX,
        REASON__COMPLEX_CLOSED
    );

    close_cash();
    remove_all_clients();
    remove_all_lifeguards();
    COMPLEX_IS_OPEN = false;
}
