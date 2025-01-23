#pragma once


#include "logging.h"
#include "data_client.h"


void log_console_pool_data(int ID, int WHO, int ACTION, int LOCATION, int REASON, void (*data)(), int status){
    __log_console(ID, WHO, ACTION, LOCATION, REASON);
    data(status);
    printf("\n");
}


// Olimpic pool
void LOG_olimpic_enter_pool(){
    log_console_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__ENTERED,
        LOCATION__OLIMPIC_POOL,
        REASON__NONE,
        disp_olimpic_data,
        STATUS_ENTER
    );
}


void LOG_olimpic_leave_pool(){
    log_console_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__OLIMPIC_POOL,
        REASON__END_OF_SWIM_TIME,
        disp_olimpic_data,
        STATUS_LEAVE
    );
}

void LOG_olimpic_enter_queue(){
    log_console_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__ENTERED,
        LOCATION__OLIMPIC_QUEUE,
        REASON__NOT_ENOUGH_SPACE,
        disp_olimpic_data,
        STATUS_NONE
    );
}

void LOG_olimpic_leave_queue(){
    log_console_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__OLIMPIC_QUEUE,
        REASON__SPACE_AVAILABLE,
        disp_olimpic_data,
        STATUS_LEAVE
    );
}