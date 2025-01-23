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


int POOL_SHMID;
int POOL_SEMID;


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


void olimpic_enter_pool(){
    OlimpicPool* pool =(OlimpicPool*) get_shared_mem(POOL_SHMID);
    pool->size += 1;
    detach_shared_mem(pool);
}

bool olimpic_space_available(){
    OlimpicPool* pool =(OlimpicPool*) get_shared_mem(POOL_SHMID);
    int cur_size = pool->size;
    detach_shared_mem(pool);
    return cur_size < POOL_SIZE[CURRENT_POOL];
}

void olimpic_set_as_cur_pool(){
    CURRENT_POOL = OLIMPIC;

    // Generate access to pool resources
    key_t key  = get_key(POOL_OLIMPIC_KEY_ID);
    POOL_SEMID = access_sem(key, SEM_POOL_NUM, 0600);
    POOL_SHMID = access_shared_mem(key, POOL_SHARED_MEM_SIZE[CURRENT_POOL], 0600);
}


void olimpic_leave_pool(){
    OlimpicPool* pool =(OlimpicPool*) get_shared_mem(POOL_SHMID);
    pool->size -= 1;
    detach_shared_mem(pool);
}


void join_olimpic_pool(){
    olimpic_set_as_cur_pool();

    // check if pool have enough size
    if(olimpic_space_available()){
        olimpic_enter_pool();
        LOG_olimpic_enter_pool();
    }
    else {
        LOG_olimpic_enter_queue();

        int status;
        while(true){
            status = USoperate_sem(POOL_SEMID, SEM_POOL_ENTER, SEM_WAIT);

            // if(USget_sem_value(pool_semid, SEM_POOL_STATUS)){
            //     log_console_with_data(getpid(),
            //         WHO__CLIENT,
            //         ACTION__LEFT,
            //         LOCATION__OLIMPIC_QUEUE,
            //         REASON__COMPLEX_CLOSED,
            //         disp_olimpic_data
            //     );

            //     exit(EXIT_SUCCESS);
            // }

            if(status == SEM_SUCCESS){
                LOG_olimpic_leave_queue();
                LOG_olimpic_enter_pool();
                olimpic_enter_pool();
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

    log_console(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__POOL_COMPLEX,
        REASON__END_OF_SWIM_TIME
    );

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