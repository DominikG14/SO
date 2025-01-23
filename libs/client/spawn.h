#pragma once

#include "data_client.h"
#include "logging.h"
#include "vars.h"


void* spawn_child(){
    while(child.WAIT_IN_CASH);
    if(child.SWIM_IN_POOL){
        log_console(getpid(),
            WHO__CHILD,
            ACTION__LEFT,
            LOCATION__CASH_QUEUE,
            REASON__PAYMENT_FINISHED
        );
    }
    else {
        log_console(getpid(),
            WHO__CHILD,
            ACTION__LEFT,
            LOCATION__CASH_QUEUE,
            REASON__CASH_CLOSED
        );

        child_leave_complex();
    }

    while(child.SWIM_IN_POOL);
    log_console(getpid(),
        WHO__CHILD,
        ACTION__LEFT,
        CURRENT_POOL,
        REASON__END_OF_SWIM_TIME
    );
    child_leave_complex();
}


void spawn_client(){
    srand(getpid());
    
    // Set child flags
    child.tid = -1;

    // Set Client data
    client.age = rand_int(CLIENT_MIN_AGE, CLIENT_MAX_AGE);
    client.swim_cap_on = rand_swim_cap();
    int child_age = rand_int(CHILD_MIN_AGE, CHILD_MAX_AGE);

    if(rand_child(client.age, child_age)){
        child.age = child_age;
        child.swim_cap_on = rand_swim_cap();
        child.WAIT_IN_CASH = true;
        child.SWIM_IN_POOL = true;

        if(child.age <= 3){
            child.diaper_on = true;
        }

        pthread_t child_tid = new_thread(spawn_child, NULL);
        child.tid = child_tid;
    }

    handle_signal(SIG_CLOSE_POOL, client_leave_complex);

    log_console_with_data(getpid(),
        WHO__CLIENT,
        ACTION__ENTERED,
        LOCATION__CASH_QUEUE,
        REASON__NONE,
        disp_client_data
    );

    if(client_has_child()){
        log_console_with_data(getpid(),
            WHO__CHILD,
            ACTION__ENTERED,
            LOCATION__CASH_QUEUE,
            REASON__NONE,
            disp_child_data
        );
    }
}