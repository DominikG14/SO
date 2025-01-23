#pragma once


#include "low.h"
#include "logging.h"


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
