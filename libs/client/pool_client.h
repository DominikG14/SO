#pragma once

#include <stdbool.h>

#include "data_client.h"
#include "logging.h"
#include "low.h"
#include "vars.h"
#include "rand_client.h"
#include "low.h"
#include "leave.h"
#include "keys_id.h"
#include "logs.h"

#include "olimpic/func.h"
#include "paddling/func.h"
#include "leisure/func.h"



void join_paddling_pool(){
    paddling_set_as_cur_pool();

    if(paddling_space_available()){
        paddling_enter_pool();
        LOG_paddling_enter_pool();
    }
    else{
        CLIENT_LOCATION = LOCATION__PADDLING_QUEUE;
        LOG_paddling_enter_queue();

        int status;
        while(true){
            status = USoperate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_WAIT);

            if(status == SEM_SUCCESS){
                LOG_paddling_leave_queue();
                paddling_enter_pool();
                LOG_paddling_enter_pool();
                break;
            }

            perror(__func__);
            exit(EXIT_FAILURE);
        }
    }

    sleep(rand_swim_time());
    paddling_leave_pool();
    LOG_paddling_leave_pool();
    child.SWIM_IN_POOL = false;
    pthread_join(child.tid, NULL);

    operate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_SIGNAL);
    client_leave_complex();
}


void join_leisure_pool(){
    leisure_set_as_cur_pool();

    if(client_has_child() && leisure_space_available_child() && leisure_below_age_avg_child()){
        leisure_enter_pool_child();
        LOG_leisure_enter_pool_child();
    }
    else if(leisure_space_available() && leisure_below_age_avg()){
        leisure_enter_pool();
        LOG_leisure_enter_pool();
    }
    else{
        CLIENT_LOCATION = LOCATION__LEISURE_QUEUE;
        if(client_has_child()) LOG_leisure_enter_queue_child();
        else LOG_leisure_enter_queue();

        int status;
        while(true){
            status = USoperate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_WAIT);

            if(status == SEM_SUCCESS){
                if(client_has_child() && leisure_space_available_child() && leisure_below_age_avg_child()){
                    LOG_leisure_leave_queue_child();
                    leisure_enter_pool_child();
                    LOG_leisure_enter_pool_child();
                    break;
                }
                else if(leisure_space_available() && leisure_below_age_avg()){
                    LOG_leisure_leave_queue();
                    leisure_enter_pool();
                    LOG_leisure_enter_pool();
                    break;
                } 
                else {
                    // operate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_SIGNAL);
                    continue;
                }
            }

            perror(__func__);
            exit(EXIT_FAILURE);
        }
    }

    
    sleep(rand_swim_time());
    if(client_has_child()){
        leisure_leave_pool_child();
        LOG_leisure_leave_pool_child();
        child.SWIM_IN_POOL = false;
        pthread_join(child.tid, NULL);
        operate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_SIGNAL);
    } else {
        leisure_leave_pool();
        LOG_leisure_leave_pool();
    }


    operate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_SIGNAL);
    LOG_leave_complex();
    exit(EXIT_SUCCESS);
}


void join_olimpic_pool(){
    olimpic_set_as_cur_pool();

    if(olimpic_space_available()){
        olimpic_enter_pool();
        LOG_olimpic_enter_pool();
    }
    else {
        CLIENT_LOCATION = LOCATION__OLIMPIC_QUEUE;
        LOG_olimpic_enter_queue();

        int status;
        while(true){
            status = USoperate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_WAIT);

            if(status == SEM_SUCCESS){
                LOG_olimpic_leave_queue();
                olimpic_enter_pool();
                LOG_olimpic_enter_pool();
                break;
            }

            perror(__func__);
            exit(EXIT_FAILURE);
        }
    }

    sleep(rand_swim_time());
    olimpic_leave_pool();
    LOG_olimpic_leave_pool();
    operate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_SIGNAL);

    LOG_leave_complex();

    exit(EXIT_SUCCESS);
}


void choose_pool(){
    if(client_has_baby()){
        join_paddling_pool();
    }

    if(client_is_underage() || client_has_child()){
        join_leisure_pool();
    }


    switch(rand_int(OLIMPIC, LEISURE)){
        case OLIMPIC:
            join_olimpic_pool();

        case LEISURE:
            join_leisure_pool();
    }
}