#pragma once

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>


const size_t MSG_Q__BUFFER_SIZE = 4096;


enum MSG_Q__STATES {
    MSG_Q__SUCCESS = 0,
    MSG_Q__FAILURE = -1,
};


struct MSG_Q__BUFFER {
    long type;
    char text[MSG_Q__BUFFER_SIZE];
} typedef MSG_Q__BUFFER;


int access_msg_q(key_t key, int flags){
    int msqid = msgget(key, flags);
    if(msqid == MSG_Q__FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return msqid;
}


void send_msg_q(int msqid, const char* text, long type){
    MSG_Q__BUFFER msg;

    msg.type = type;
    strcpy(msg.text, text);

    int status = msgsnd(msqid, &msg, sizeof(msg.text), 0);
    if(status == MSG_Q__FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }
}


char* get_msg_q(int msqid, long type){
    MSG_Q__BUFFER msg;
    // msg.type = type;

    int status = msgrcv(msqid, &msg, sizeof(msg.text), type, 0);
    if(status == MSG_Q__FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return msg.text; // CHECK: (msg.text) ?
}


void delete_msg_q(int msqid){
    int status = msgctl(msqid, IPC_RMID, NULL);
    if(status == MSG_Q__FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }
}