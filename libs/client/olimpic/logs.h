#pragma once

#include "logging.h"
#include "client/data_client.h"
#include "client/logs.h"


void LOG_olimpic_enter_pool(){
    log_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__ENTERED,
        LOCATION__OLIMPIC_POOL,
        REASON__NONE,
        olimpic_data,
        STATUS_ENTER
    );
}


void LOG_olimpic_leave_pool(){
    log_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__OLIMPIC_POOL,
        REASON__END_OF_SWIM_TIME,
        olimpic_data,
        STATUS_LEAVE
    );
}

void LOG_olimpic_enter_queue(){
    log_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__ENTERED,
        LOCATION__OLIMPIC_QUEUE,
        REASON__NOT_ENOUGH_SPACE,
        olimpic_data,
        STATUS_NONE
    );
}

void LOG_olimpic_leave_queue(){
    log_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__OLIMPIC_QUEUE,
        REASON__SPACE_AVAILABLE,
        olimpic_data,
        STATUS_LEAVE
    );
}