#pragma once

#include "global.h"

#define FILE_SIZE 4096


enum GLOBAL_STATE {
    SUCCESS = 0,
    FAILURE = -1,
};


// -------------------- Messages Queue --------------------
struct MsqBuffer {
	long mtype;
	int  mvalue;
} typedef MsqBuffer;


int access_msq(key_t key, int flags){
    int msqid = msgget(key, flags);
    if(msqid == FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return msqid;
}


void send_msq(int msqid, long type){
    MsqBuffer msg;
    msg.mtype = type;

    int status = msgsnd(msqid, &msg, sizeof(msg.mvalue), 0);
    if(status == FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }
}


void get_msq(int msqid, long type){
    MsqBuffer msg;

    int status = msgrcv(msqid, &msg, sizeof(msg.mvalue), type, 0);
    if(status == FAILURE && errno != EINTR){
        perror(__func__);
        exit(EXIT_FAILURE);
    }
}


int get_msq_count(int msqid){
    struct msqid_ds buf;

    int status = msgctl(msqid, IPC_STAT, &buf);
    if(status == FAILURE) {
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return buf.msg_qnum;
}


// -------------------- Keys --------------------
const char* KEY_PATH = ".";

key_t get_key(int id){
    key_t key = ftok(KEY_PATH, id);
    if(key == FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return key;
}


// -------------------- Threads --------------------
pthread_t new_thread(void* (*thread_func)(void*), void* thread_args){
    pthread_t tid;

    int status = pthread_create(&tid, NULL, thread_func, thread_args);
    if(status == FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return tid;
}


// -------------------- Files --------------------
char* alloc(){
    char* result =(char*) malloc(FILE_SIZE * sizeof(char));
    if(result == NULL){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return result;
}


char* read_file(const char* filepath, size_t* result_size){
    // File opening
    int fd = open(filepath, O_RDONLY);
    if(fd == FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    // Resulting text
    char* result = NULL;

    // Read file content
    size_t bytes_read;
    size_t curr_size;
    size_t total_bytes = 0;
    char buffer[FILE_SIZE];
    while((bytes_read = read(fd, buffer, FILE_SIZE)) > 0){

        if(result == NULL){
            result = alloc();
            curr_size = 1;
        } else {
            curr_size++;
            result =(char*) realloc(result, FILE_SIZE * curr_size * sizeof(char));
        }

        strncat(result, buffer, bytes_read);
        total_bytes += bytes_read;
    }

    // Changing to optimal size
    result =(char*) realloc(result, total_bytes * sizeof(char));
    result[total_bytes] = '\0';
    if(result_size){
        *result_size = total_bytes;
    }

    // Closing the file
    close(fd);

    return result;
}


void write_file(char* filepath, char* text){
    int fd = open(filepath, O_CREAT|O_WRONLY, 0600);
    if(fd == FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    write(fd, text, strlen(text));
    close(fd);
}


void append_file(char* filepath, char* text){
    int fd = open(filepath, O_CREAT|O_WRONLY|O_APPEND, 0600);
    if(fd == FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    write(fd, text, strlen(text));
    close(fd);
}