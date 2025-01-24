#pragma once

#include "logging.h"
#include "client/data_client.h"


void LOG_leisure_enter_pool(){
    log_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__ENTERED,
        LOCATION__LEISURE_POOL,
        REASON__NONE,
        leisure_data,
        STATUS_ENTER
    );
}

void LOG_leisure_enter_pool_child(){
    LOG_leisure_enter_pool();

    log_pool_data(getpid(),
        WHO__CHILD,
        ACTION__ENTERED,
        LOCATION__LEISURE_POOL,
        REASON__NONE,
        leisure_data,
        STATUS_ENTER
    );
}


void LOG_leisure_enter_queue(){
    log_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__ENTERED,
        LOCATION__LEISURE_QUEUE,
        REASON__NOT_ENOUGH_SPACE,
        leisure_data,
        STATUS_NONE
    );
}

void LOG_leisure_enter_queue_child(){
    LOG_leisure_enter_queue();

    log_pool_data(getpid(),
        WHO__CHILD,
        ACTION__ENTERED,
        LOCATION__LEISURE_QUEUE,
        REASON__NOT_ENOUGH_SPACE,
        leisure_data,
        STATUS_NONE
    );
}


void LOG_leisure_leave_pool(){
    log_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__LEISURE_POOL,
        REASON__END_OF_SWIM_TIME,
        leisure_data,
        STATUS_LEAVE
    );
}

void LOG_leisure_leave_pool_child(){
    LOG_leisure_leave_pool();

    log_pool_data(getpid(),
        WHO__CHILD,
        ACTION__LEFT,
        LOCATION__LEISURE_POOL,
        REASON__END_OF_SWIM_TIME,
        leisure_data,
        STATUS_LEAVE
    );
}


void LOG_leisure_leave_queue(){
    log_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__LEISURE_QUEUE,
        REASON__SPACE_AVAILABLE,
        leisure_data,
        STATUS_LEAVE
    );
}

void LOG_leisure_leave_queue_child(){
    LOG_leisure_leave_queue();

    log_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__LEISURE_QUEUE,
        REASON__SPACE_AVAILABLE,
        leisure_data,
        STATUS_LEAVE
    );
}