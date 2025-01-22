#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>

#include "client.h"
#include "low/signal.h"
#include "low/sem.h"
#include "low/files.h"
#include "low/msq.h"
#include "low/key.h"
#include "keys_id.h"


char* getpid_str(){
    char* pid_str =(char*) malloc(20);
    if(pid_str == NULL){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    sprintf(pid_str, "./tmp/%d", getpid());
    return pid_str;
}


void leave_pool(){
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
    char* pid_str = getpid_str();
    send_msq(cash_msqid, pid_str, 1);
    status = mkfifo(pid_str, 0600);
    if(status == -1){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    int fifo_fd = open(pid_str, O_RDONLY);
    if( fifo_fd == FILE_FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    char buffer[100];
    while(1) {
        ssize_t bytes_read = read(fifo_fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("%d: Client recived: %s\n", getpid(), buffer);
            break;
        }
    }

    close(fifo_fd);
    unlink(pid_str);
    
    free(pid_str), pid_str = NULL;
    printf("%d: Client left the queue\n", getpid());
}


int main(){
    setup();

    Client client;

    client.age = rand_int(11, 70);
    client.swim_cap_on = rand_swim_cap();
    int child_age = rand_int(1, 10);

    printf("%d: swim_cap: %d, age %d", getpid(), client.swim_cap_on, client.age);
    if(rand_child(client.age, child_age)){
        printf(", child: %d", child_age);
    }
    printf("\n");

    signal(SIG_CLOSE_POOL, leave_pool);
    wait_in_cash_queue();

    while(true){
    }

    return 0;
}