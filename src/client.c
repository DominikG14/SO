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


void* spawn_child(){
    while(child.WAIT_IN_CASH);
    if(child.SWIM_IN_POOL){
        printf_clr(green, "%d", getpid());
        printf(": Child  (");
        printf_clr(yellow, "Cash");
        printf(")\n");

        printf("%ld: Child of %d left the cash\n", child.tid, getpid());
    }
    else {
        printf_clr(green, "%d", getpid());
        printf(": Child left (");
        printf_clr(yellow, "Cash");
        printf_clr(red, " is closed");
        printf(")\n");

        pthread_exit(EXIT_SUCCESS);
    }

    while(child.SWIM_IN_POOL);
    printf_clr(green, "%d", getpid());
    printf(" -> ");
    printf_clr(green, "%.6ld", child.tid);
    printf(": Child left (");
    printf_clr(cyan, "Pool");
    printf_clr(red, " is closed");
    printf(")\n");
    pthread_exit(EXIT_SUCCESS);
}


void leave_pool(){
    if(child.tid != -1){
        child.SWIM_IN_POOL = false;
        pthread_join(child.tid, NULL);
    }
    printf_clr(blue, "%d", getpid());
    printf(": Client left the pool\n");

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

    printf("%d: swim_cap: %d, age %d", getpid(), client.swim_cap_on, client.age);
    if(rand_child(client.age, child_age)){
        printf(", child: %d", child_age);

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
    printf("\n");

    handle_signal(SIG_CLOSE_POOL, leave_pool);
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
            if(child.tid != -1){
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
            // printf("%d: Client recived: %s\n", getpid(), buffer);
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
            NONE
        ); 

        int status;
        while(true){
            status = USoperate_sem(pool_semid, SEM_POOL_ENTER, SEM_WAIT);

            if(USget_sem_value(pool_semid, SEM_CASH_STATUS)){
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
                    NONE
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
        NONE
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
}


void choose_pool(){
    if(child.tid != -1 && child.age <= 5){
        join_paddling_pool();
    }

    leave_pool();
}


int main(){
    spawn_client();
    wait_in_cash_queue();
    choose_pool();

    return 0;
}