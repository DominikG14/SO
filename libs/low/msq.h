#pragma once
#define MSQ_BUFFER_SIZE 4096

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>


enum MSG_Q__STATES {
    MSQ_SUCCESS = 0,
    MSQ_FAILURE = -1,
};


struct MSQ_BUFFER {
    long type;
    char text[MSQ_BUFFER_SIZE];
} typedef MSQ_BUFFER;


int access_msq(key_t key, int flags){
    int msqid = msgget(key, flags);
    if(msqid == MSQ_FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return msqid;
}


void send_msq(int msqid, const char* text, long type){
    MSQ_BUFFER msg;

    msg.type = type;
    strcpy(msg.text, text);

    int status = msgsnd(msqid, &msg, sizeof(msg.text), 0);
    if(status == MSQ_FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }
}


char* get_msq(int msqid, long type){
    MSQ_BUFFER msg;

    int status = msgrcv(msqid, &msg, sizeof(msg.text), type, 0);
    if(status == MSQ_FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    char* message =(char*) malloc(MSQ_BUFFER_SIZE);
    strcpy(message, msg.text);
    return message;
}


int USget_msq(int msqid, long type, char** message){
    MSQ_BUFFER msg;

    int status = msgrcv(msqid, &msg, sizeof(msg.text), type, 0);

    if(*message){
        free(*message), *message = NULL;
    }
    
    *message =(char*) malloc(MSQ_BUFFER_SIZE);
    if(*message == NULL){
        perror(__func__);
        exit(EXIT_FAILURE);
    }
    strcpy(*message, msg.text);
    return status;
}


void delete_msq(int msqid){
    int status = msgctl(msqid, IPC_RMID, NULL);
    if(status == MSQ_FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }
}