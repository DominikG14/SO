#pragma once

#include "logging.h"
#include "vars.h"


void LOG_enter_guarder_pool(){
    log_console(getpid(),
        WHO__LIFEGUARD,
        ACTION__ENTERED,
        GUARDED_POOL,
        REASON__NONE
    );
}


void LOG_leave_guarded_pool(){
    log_console(getpid(),
        WHO__LIFEGUARD,
        ACTION__LEFT,
        GUARDED_POOL,
        REASON__COMPLEX_CLOSED
    );
}


void LOG_leave_complex(){
    log_console(getpid(),
        WHO__LIFEGUARD,
        ACTION__LEFT,
        LOCATION__POOL_COMPLEX,
        REASON__COMPLEX_CLOSED
    );
}


void LOG_closed_pool(){
    log_console(getpid(),
        WHO__LIFEGUARD,
        ACTION__CLOSED,
        GUARDED_POOL,
        REASON__NONE
    );
}


void LOG_pool_reopened(){
    log_console(getpid(),
        WHO__LIFEGUARD,
        ACTION__OPENED,
        GUARDED_POOL,
        REASON__NONE
    );
}