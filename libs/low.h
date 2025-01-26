#pragma once

#include "global.h"


enum GLOBAL_STATE {
    SUCCESS = 0,
    FAILURE = -1,
};


// Processes
const char* PS_CLIENT_PATH = "build/client";
const char* PS_CLIENT_NAME  = "./build/client";

const char* PS_CASHIER_PATH = "build/cashier"; 
const char* PS_CASHIER_NAME  = "./build/cashier";

const char* PS_LIFEGUARD_PATH = "build/lifeguard"; 
const char* PS_LIFEGUARD_NAME  = "./build/lifeguard";

// Keys
const char* KEY_PATH = ".";


key_t get_key(int id){
    key_t key = ftok(KEY_PATH, id);
    if(key == FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return key;
}


// Threads
pthread_t new_thread(void* (*thread_func)(void*), void* thread_args){
    pthread_t tid;

    int status = pthread_create(&tid, NULL, thread_func, thread_args);
    if(status == FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return tid;
}


// Files
const int FILE_SIZE = 4096;


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