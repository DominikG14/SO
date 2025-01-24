#pragma once

#include "logging.h"
#include "client/data_client.h"
#include "client/logs.h"


void LOG_paddling_enter_pool(){
    log_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__ENTERED,
        LOCATION__PADDLING_POOL,
        REASON__NONE,
        paddling_data,
        STATUS_ENTER
    );

    log_pool_data(getpid(),
        WHO__CHILD,
        ACTION__ENTERED,
        LOCATION__PADDLING_POOL,
        REASON__NONE,
        paddling_data,
        STATUS_ENTER
    );
}

void LOG_paddling_enter_queue(){
    log_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__ENTERED,
        LOCATION__PADDLING_QUEUE,
        REASON__NOT_ENOUGH_SPACE,
        paddling_data,
        STATUS_NONE
    );

    log_pool_data(getpid(),
        WHO__CHILD,
        ACTION__ENTERED,
        LOCATION__PADDLING_QUEUE,
        REASON__NOT_ENOUGH_SPACE,
        paddling_data,
        STATUS_NONE
    );
}

void LOG_paddling_leave_queue(){
    log_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__PADDLING_QUEUE,
        REASON__SPACE_AVAILABLE,
        paddling_data,
        STATUS_LEAVE
    );

    log_pool_data(getpid(),
        WHO__CHILD,
        ACTION__LEFT,
        LOCATION__PADDLING_QUEUE,
        REASON__SPACE_AVAILABLE,
        paddling_data,
        STATUS_LEAVE
    );
}

void LOG_paddling_leave_pool(){
    log_pool_data(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__PADDLING_POOL,
        REASON__END_OF_SWIM_TIME,
        paddling_data,
        STATUS_LEAVE
    );

    log_pool_data(getpid(),
        WHO__CHILD,
        ACTION__LEFT,
        LOCATION__PADDLING_POOL,
        REASON__END_OF_SWIM_TIME,
        paddling_data,
        STATUS_LEAVE
    );
}