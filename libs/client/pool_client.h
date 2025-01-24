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


    key_t key = get_key(POOL_LEISURE_KEY_ID);
    POOL_SEMID = access_sem(key, SEM_POOL_NUM, 0600);
    POOL_SHMID = access_shared_mem(key, POOL_SHARED_MEM_SIZE[LEISURE], 0600);
    LeisurePool* pool =(LeisurePool*) get_shared_mem(POOL_SHMID);


    if(client_has_child()){
        if(pool->size < POOL_SIZE[LEISURE] - 1 && leisure_age_avg(pool, client.age + child.age, 2) <= POOL_LEISURE_AGE_AVG){
            WAIT_IN_QUEUE = false;
            pool->size += 2;
            pool->age_sum += client.age + child.age;
            detach_shared_mem(pool);
        }
    } else if(pool->size < POOL_SIZE[LEISURE] && leisure_age_avg(pool, client.age, 1) <= POOL_LEISURE_AGE_AVG){
            WAIT_IN_QUEUE = false;
            pool->size += 1;
            pool->age_sum += client.age;
            detach_shared_mem(pool);
    }

    if(WAIT_IN_QUEUE){

        log_console_with_data(getpid(),
            WHO__CLIENT,
            ACTION__ENTERED,
            LOCATION__LEISURE_QUEUE,
            REASON__NONE,
            disp_leisure_data
        );

        int status;
        while(true){
            status = USoperate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_WAIT);

            if(USget_sem_value(POOL_SEMID, SEM_POOL_STATUS)){
                log_console_with_data(getpid(),
                    WHO__CLIENT,
                    ACTION__LEFT,
                    LOCATION__LEISURE_QUEUE,
                    REASON__COMPLEX_CLOSED,
                    disp_leisure_data
                );

                exit(EXIT_SUCCESS);
            }

            if(status == SEM_SUCCESS){
                pool =(LeisurePool*) get_shared_mem(POOL_SHMID);

                if(client_has_child()){
                    if(pool->size < POOL_SIZE[LEISURE] - 1 && leisure_age_avg(pool, client.age + child.age, 2) <= POOL_LEISURE_AGE_AVG){
                        WAIT_IN_QUEUE = false;
                        pool->size += 2;
                        pool->age_sum += client.age + child.age;
                        detach_shared_mem(pool);

                        log_console_with_data(getpid(),
                            WHO__CLIENT,
                            ACTION__ENTERED,
                            LOCATION__LEISURE_POOL,
                            REASON__NONE,
                            disp_leisure_data
                        );

                        break;
                    } 
                } else if(pool->size < POOL_SIZE[LEISURE] && leisure_age_avg(pool, client.age, 1) <= POOL_LEISURE_AGE_AVG){
                    log_console_with_data(getpid(),
                        WHO__CLIENT,
                        ACTION__ENTERED,
                        LOCATION__LEISURE_POOL,
                        REASON__NONE,
                        disp_leisure_data
                    );

                    WAIT_IN_QUEUE = false;
                    pool->size += 1;
                    pool->age_sum += client.age;
                    detach_shared_mem(pool);
                    break;
                } else {
                    detach_shared_mem(pool);
                    continue;
                }


                log_console_with_data(getpid(),
                    WHO__CLIENT,
                    ACTION__LEFT,
                    LOCATION__LEISURE_QUEUE,
                    REASON__NONE,
                    disp_leisure_data
                );

                break;
            }

            perror(__func__);
            exit(EXIT_FAILURE);
        }
    }

    
    sleep(rand_swim_time());
    pool =(LeisurePool*) get_shared_mem(POOL_SHMID);
    if(client_has_child()){
        pool->size -= 2;
        pool->age_sum -= client.age + child.age;
        detach_shared_mem(pool);
    } else {
        pool->size -= 1;
        pool->age_sum -= client.age;
        detach_shared_mem(pool);
    }

    log_console_with_data(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__LEISURE_POOL,
        REASON__END_OF_SWIM_TIME,
        disp_leisure_data
    );

    if(client_has_child()){
        operate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_SIGNAL);
        sleep(1);
    }
    operate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_SIGNAL);

    client_leave_complex();
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