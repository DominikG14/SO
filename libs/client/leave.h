#pragma once

#include "low.h"
#include "logging.h"


void child_leave_complex(){
    log_console(getpid(),
        WHO__CHILD,
        ACTION__LEFT,
        LOCATION__POOL_COMPLEX,
        REASON__COMPLEX_CLOSED
    );

    pthread_exit(EXIT_SUCCESS);
}



void client_leave_complex(){
    if(client_has_child()){
        child.SWIM_IN_POOL = false;
        pthread_join(child.tid, NULL);
    }

    log_console(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        CLIENT_LOCATION,
        REASON__COMPLEX_CLOSED
    );

    log_console(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__POOL_COMPLEX,
        REASON__COMPLEX_CLOSED
    );

    exit(EXIT_SUCCESS);
}