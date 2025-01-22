#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>

#include "client.h"
#include "keys_id.h"

#include "low/signal.h"
#include "low/sem.h"
#include "low/files.h"
#include "low/msq.h"
#include "low/key.h"
#include "low/threads.h"


Client client;
Child  child;


void spawn_client(){
    srand(getpid());
    
    // Set child flags
    child.tid = -1;

    // Set Client data
    client.age = rand_int(11, 70);
    client.swim_cap_on = rand_swim_cap();
    int child_age = rand_int(1, 10);

    printf("%d: swim_cap: %d, age %d", getpid(), client.swim_cap_on, client.age);
    if(rand_child(client.age, child_age)){
        printf(", child: %d", child_age);

        child.age = child_age;
        child.swim_cap_on = rand_swim_cap();
        child.WAIT_IN_CASH = true;

        if(child.age <= 3){
            child.diaper_on = true;
        }

        client.child = child;

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


void leave_pool(){
    if(client.child.tid != -1){

    }
    printf("%d: Client left the pool\n", getpid());
    exit(EXIT_SUCCESS);
}


void wait_in_cash_queue(){
    key_t key = get_key(CASH_KEY_ID);
    int cash_semid = access_sem(key, 1, 0600);

    int status;
    while(true){
        status = USoperate_sem(cash_semid, SEM_CASH_PAYMENT, SEM_WAIT);

        if(USget_sem_value(cash_semid, SEM_CASH_STATUS)){
            printf("%d: Cash is closed, Client leaving\n", getpid());
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
            printf("%d: Client recived: %s\n", getpid(), buffer);
            break;
        }
    }

    child.WAIT_IN_CASH = false;

    close(fd_fifo);
    unlink(pid_str);
    
    free(pid_str), pid_str = NULL;

    printf("%d: Client left the cash\n", getpid());
}


void* spawn_child(){
    while(child.WAIT_IN_CASH);
    printf("%ld: Child of %d left the cash\n", child.tid, getpid());

    while(child.SWIM_IN_POOL);
    printf("%ld: Child of %d left the pool\n", child.tid, getpid());
    pthread_exit(EXIT_SUCCESS);
}


int main(){
    spawn_client();
    wait_in_cash_queue();

    while(true){

    }

    return 0;
}