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


void join_paddling_pool(){
    bool WAIT_IN_QUEUE = true;
    CURRENT_POOL = PADDLING;

    key_t key = get_key(POOL_PADDLING_KEY_ID);

    int pool_semid = access_sem(key, SEM_POOL_NUM, 0600);

    int pool_shmid = access_shared_mem(key, POOL_SHARED_MEM_SIZE[PADDLING], 0600);
    PaddlingPool* pool =(PaddlingPool*) get_shared_mem(pool_shmid);


    if(pool->size / 2 < POOL_SIZE[PADDLING] / 2){
        WAIT_IN_QUEUE = false;
        pool->size += 2;
        detach_shared_mem(pool);
    }


    if(WAIT_IN_QUEUE){

        log_console_with_data(getpid(),
            WHO__CLIENT,
            ACTION__ENTERED,
            LOCATION__PADDLING_QUEUE,
            REASON__NONE,
            disp_paddling_data
        );
        log_console_with_data(getpid(),
            WHO__CHILD,
            ACTION__ENTERED,
            LOCATION__PADDLING_QUEUE,
            REASON__NONE,
            disp_paddling_data
        );

        int status;
        while(true){
            status = USoperate_sem(pool_semid, SEM_POOL_ENTER, SEM_WAIT);

            if(USget_sem_value(pool_semid, SEM_POOL_STATUS)){
                child.SWIM_IN_POOL = false;
                child.WAIT_IN_CASH = false;
                
                pthread_join(child.tid, NULL);

                log_console_with_data(getpid(),
                    WHO__CLIENT,
                    ACTION__LEFT,
                    LOCATION__PADDLING_QUEUE,
                    REASON__COMPLEX_CLOSED,
                    disp_paddling_data
                );

                exit(EXIT_SUCCESS);
            }

            if(status == SEM_SUCCESS){
                
                log_console_with_data(getpid(),
                    WHO__CLIENT,
                    ACTION__LEFT,
                    LOCATION__PADDLING_QUEUE,
                    REASON__NONE,
                    disp_paddling_data
                ); 

                break;
            }

            perror(__func__);
            exit(EXIT_FAILURE);
        }
    }

    log_console_with_data(getpid(),
        WHO__CLIENT,
        ACTION__ENTERED,
        LOCATION__PADDLING_POOL,
        REASON__NONE,
        disp_paddling_data
    );

    sleep(rand_swim_time());
    pool =(PaddlingPool*) get_shared_mem(pool_shmid);
    pool->size -= 2;
    detach_shared_mem(pool);

    log_console_with_data(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__PADDLING_POOL,
        REASON__END_OF_SWIM_TIME,
        disp_paddling_data
    );

    operate_sem(pool_semid, SEM_POOL_ENTER, SEM_SIGNAL);
    client_leave_complex();
}


void join_leisure_pool(){
    bool WAIT_IN_QUEUE = true;
    CURRENT_POOL = LEISURE;

    key_t key = get_key(POOL_LEISURE_KEY_ID);

    int pool_semid = access_sem(key, SEM_POOL_NUM, 0600);

    int pool_shmid = access_shared_mem(key, POOL_SHARED_MEM_SIZE[LEISURE], 0600);
    LeisurePool* pool =(LeisurePool*) get_shared_mem(pool_shmid);


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
            status = USoperate_sem(pool_semid, SEM_POOL_ENTER, SEM_WAIT);

            if(USget_sem_value(pool_semid, SEM_POOL_STATUS)){
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
                pool =(LeisurePool*) get_shared_mem(pool_shmid);

                if(client_has_child()){
                    if(pool->size < POOL_SIZE[LEISURE] - 1 && leisure_age_avg(pool, client.age + child.age, 2) <= POOL_LEISURE_AGE_AVG){
                        WAIT_IN_QUEUE = false;
                        pool->size += 2;
                        pool->age_sum += client.age + child.age;
                        detach_shared_mem(pool);
                        break;
                    } 
                } else if(pool->size < POOL_SIZE[LEISURE] && leisure_age_avg(pool, client.age, 1) <= POOL_LEISURE_AGE_AVG){
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

    log_console_with_data(getpid(),
        WHO__CLIENT,
        ACTION__ENTERED,
        LOCATION__LEISURE_POOL,
        REASON__NONE,
        disp_leisure_data
    );

    
    sleep(rand_swim_time());
    pool =(LeisurePool*) get_shared_mem(pool_shmid);
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
        operate_sem(pool_semid, SEM_POOL_ENTER, SEM_SIGNAL);
        sleep(1);
    }
    operate_sem(pool_semid, SEM_POOL_ENTER, SEM_SIGNAL);

    client_leave_complex();
}


void join_olimpic_pool(){
    bool WAIT_IN_QUEUE = true;
    CURRENT_POOL = OLIMPIC;

    key_t key = get_key(POOL_OLIMPIC_KEY_ID);

    int pool_semid = access_sem(key, SEM_POOL_NUM, 0600);

    int pool_shmid = access_shared_mem(key, POOL_SHARED_MEM_SIZE[OLIMPIC], 0600);
    OlimpicPool* pool =(OlimpicPool*) get_shared_mem(pool_shmid);


    if(pool->size  < POOL_SIZE[OLIMPIC]){
        WAIT_IN_QUEUE = false;
        pool->size += 1;
        detach_shared_mem(pool);
    }


    if(WAIT_IN_QUEUE){

        log_console_with_data(getpid(),
            WHO__CLIENT,
            ACTION__ENTERED,
            LOCATION__OLIMPIC_QUEUE,
            REASON__NONE,
            disp_olimpic_data
        );

        int status;
        while(true){
            status = USoperate_sem(pool_semid, SEM_POOL_ENTER, SEM_WAIT);

            if(USget_sem_value(pool_semid, SEM_POOL_STATUS)){
                log_console_with_data(getpid(),
                    WHO__CLIENT,
                    ACTION__LEFT,
                    LOCATION__OLIMPIC_QUEUE,
                    REASON__COMPLEX_CLOSED,
                    disp_olimpic_data
                );

                exit(EXIT_SUCCESS);
            }

            if(status == SEM_SUCCESS){
                
                log_console_with_data(getpid(),
                    WHO__CLIENT,
                    ACTION__LEFT,
                    LOCATION__OLIMPIC_QUEUE,
                    REASON__NONE,
                    disp_olimpic_data
                ); 

                break;
            }

            perror(__func__);
            exit(EXIT_FAILURE);
        }
    }

    log_console_with_data(getpid(),
        WHO__CLIENT,
        ACTION__ENTERED,
        LOCATION__OLIMPIC_POOL,
        REASON__NONE,
        disp_olimpic_data
    );

    sleep(rand_swim_time());
    pool =(OlimpicPool*) get_shared_mem(pool_shmid);
    pool->size -= 1;
    detach_shared_mem(pool);

    log_console_with_data(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__OLIMPIC_POOL,
        REASON__END_OF_SWIM_TIME,
        disp_olimpic_data
    );

    operate_sem(pool_semid, SEM_POOL_ENTER, SEM_SIGNAL);
    client_leave_complex();
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