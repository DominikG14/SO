#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>

#include "client.h"
#include "keys_id.h"
#include "color.h"
#include "pool.h"
#include "logging.h"

#include "low/signal.h"
#include "low/sem.h"
#include "low/shared_mem.h"
#include "low/files.h"
#include "low/msq.h"
#include "low/key.h"
#include "low/threads.h"


Client client;
Child  child;
int CURRENT_POOL;


double leisure_age_avg(LeisurePool* pool, int additional_age, int additional_size){
    int age_sum = pool->age_sum + additional_age;
    int size_sum = pool->size + additional_size;

    double age_avg =(double) age_sum / size_sum;

    return age_avg;
}


void disp_leisure_data(){
    key_t key = get_key(POOL_LEISURE_KEY_ID);
    int pool_shmid = access_shared_mem(key, POOL_SHARED_MEM_SIZE[LEISURE], 0600);
    LeisurePool* pool =(LeisurePool*) get_shared_mem(pool_shmid);

    // Pool size
    printf_clr(cyan, "| ");
    printf("size: %ld", pool->size);

    // Pool age sum
    printf_clr(cyan, " | ");
    printf("age_sum: %ld", pool->age_sum);

    // Pool age avg
    printf_clr(cyan, " | ");
    printf("age_avg: %lf", leisure_age_avg(pool, 0, 0));

    // End
    printf_clr(cyan, " |");

    detach_shared_mem(pool);
}


bool client_is_underage(){
    return client.age < 18; 
}


bool client_has_child(){
    return child.tid != -1;
}


bool client_has_baby(){
    return client_has_child() && child.age <= 3;
}


void disp_client_data(){
    // Client Age
    printf_clr(blue, "| ");
    printf("age: %d", client.age);

    // Client swim cap
    printf_clr(blue, " | ");
    printf("swim_cap_on: ");
    if(client.swim_cap_on) printf_clr(green, "yes");
    else printf_clr(red, "no");

    // Client child
    printf_clr(blue, " | ");
    printf("with_child: ");
    if(client_has_child()) printf_clr(green, "yes");
    else printf_clr(red, "no");

    // End
    printf_clr(blue, " |");
}


void disp_child_data(){
    // Child Parent

    // Child Age
    printf_clr(green, "| ");
    printf("age: %d", child.age);

    // Child swim cap
    printf_clr(green, " | ");
    printf("swim_cap_on: ");
    if(child.swim_cap_on) printf_clr(green, "yes");
    else printf_clr(red, "no");

    // Child diaper
    printf_clr(green, " | ");
    printf("diaper_on: ");
    if(child.diaper_on) printf_clr(green, "yes");
    else printf_clr(red, "no");

    // End
    printf_clr(green, " |");
}


void child_leave_complex(){
    log_console(getpid(),
        WHO__CHILD,
        ACTION__LEFT,
        LOCATION__POOL_COMPLEX,
        REASON__COMPLEX_CLOSED
    );

    pthread_exit(EXIT_SUCCESS);
}


void* spawn_child(){
    log_console_with_data(getpid(),
        WHO__CHILD,
        ACTION__ENTERED,
        LOCATION__CASH_QUEUE,
        REASON__NONE,
        disp_child_data
    );



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


void client_leave_complex(){
    if(client_has_child()){
        child.SWIM_IN_POOL = false;
        pthread_join(child.tid, NULL);
    }

    log_console(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__POOL_COMPLEX,
        REASON__COMPLEX_CLOSED // TODO: Probably need to change in the future
    );

    exit(EXIT_SUCCESS);
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
}


char* get_tmp(int id){
    char* id_str =(char*) malloc(20);
    if(id_str == NULL){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    sprintf(id_str, "./tmp/%d", id);
    return id_str;
}


void wait_in_cash_queue(){
    key_t key = get_key(CASH_KEY_ID);
    int cash_semid = access_sem(key, SEM_CASH_NUM, 0600);

    int status;
    while(true){
        status = USoperate_sem(cash_semid, SEM_CASH_PAYMENT, SEM_WAIT);

        if(USget_sem_value(cash_semid, SEM_CASH_STATUS)){
            if(client_has_child()){
                child.SWIM_IN_POOL = false;
                child.WAIT_IN_CASH = false;
                pthread_join(child.tid, NULL);
            }

            log_console(getpid(),
                WHO__CLIENT,
                ACTION__LEFT,
                LOCATION__CASH_QUEUE,
                REASON__CASH_CLOSED
            );

            exit(EXIT_SUCCESS);
        }

        if(status == SEM_SUCCESS){
            break;
        }

        perror(__func__);
        exit(EXIT_FAILURE);
    }

    int cash_msqid = access_msq(key, 0200);
    char* pid_str = get_tmp(getpid());
    send_msq(cash_msqid, pid_str, 1);
    status = mkfifo(pid_str, 0600);
    if(status == -1){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    int fd_fifo = open(pid_str, O_RDONLY);
    if(fd_fifo == FILE_FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    char buffer[100];
    while(true){
        ssize_t bytes_read = read(fd_fifo, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';

            log_console(getpid(),
                WHO__CLIENT,
                ACTION__RECIVED_BILL,
                LOCATION__CASH_QUEUE,
                REASON__NONE
            );

            break;
        }
    }
    child.WAIT_IN_CASH = false;

    close(fd_fifo);
    unlink(pid_str);
    
    free(pid_str), pid_str = NULL;

    log_console(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__CASH_QUEUE,
        REASON__PAYMENT_FINISHED
    );
}


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

        log_console(getpid(),
            WHO__CLIENT,
            ACTION__ENTERED,
            LOCATION__PADDLING_QUEUE,
            REASON__NONE
        );
        log_console(getpid(),
            WHO__CHILD,
            ACTION__ENTERED,
            LOCATION__PADDLING_QUEUE,
            REASON__NONE
        );

        int status;
        while(true){
            status = USoperate_sem(pool_semid, SEM_POOL_ENTER, SEM_WAIT);

            if(USget_sem_value(pool_semid, SEM_POOL_STATUS)){
                child.SWIM_IN_POOL = false;
                child.WAIT_IN_CASH = false;
                
                pthread_join(child.tid, NULL);

                log_console(getpid(),
                    WHO__CLIENT,
                    ACTION__LEFT,
                    LOCATION__PADDLING_QUEUE,
                    REASON__COMPLEX_CLOSED // Probably need to change in the future
                );

                exit(EXIT_SUCCESS);
            }

            if(status == SEM_SUCCESS){
                
                log_console(getpid(),
                    WHO__CLIENT,
                    ACTION__LEFT,
                    LOCATION__PADDLING_QUEUE,
                    REASON__NONE
                ); 

                break;
            }

            perror(__func__);
            exit(EXIT_FAILURE);
        }
    }

    log_console(getpid(),
        WHO__CLIENT,
        ACTION__ENTERED,
        LOCATION__PADDLING_POOL,
        REASON__NONE
    );

    sleep(rand_swim_time());
    pool =(PaddlingPool*) get_shared_mem(pool_shmid);
    pool->size -= 2;
    detach_shared_mem(pool);

    log_console(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__PADDLING_POOL,
        REASON__END_OF_SWIM_TIME
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

        log_console(getpid(),
            WHO__CLIENT,
            ACTION__ENTERED,
            LOCATION__OLIMPIC_QUEUE,
            REASON__NONE
        );

        int status;
        while(true){
            status = USoperate_sem(pool_semid, SEM_POOL_ENTER, SEM_WAIT);

            if(USget_sem_value(pool_semid, SEM_POOL_STATUS)){
                log_console(getpid(),
                    WHO__CLIENT,
                    ACTION__LEFT,
                    LOCATION__OLIMPIC_QUEUE,
                    REASON__COMPLEX_CLOSED
                );

                exit(EXIT_SUCCESS);
            }

            if(status == SEM_SUCCESS){
                
                log_console(getpid(),
                    WHO__CLIENT,
                    ACTION__LEFT,
                    LOCATION__OLIMPIC_QUEUE,
                    REASON__NONE
                ); 

                break;
            }

            perror(__func__);
            exit(EXIT_FAILURE);
        }
    }

    log_console(getpid(),
        WHO__CLIENT,
        ACTION__ENTERED,
        LOCATION__OLIMPIC_POOL,
        REASON__NONE
    );

    sleep(rand_swim_time());
    pool =(OlimpicPool*) get_shared_mem(pool_shmid);
    pool->size -= 1;
    detach_shared_mem(pool);

    log_console(getpid(),
        WHO__CLIENT,
        ACTION__LEFT,
        LOCATION__OLIMPIC_POOL,
        REASON__END_OF_SWIM_TIME
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


int main(){
    spawn_client();
    wait_in_cash_queue();
    choose_pool();

    return 0;
}