#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


const int FILE_SIZE = 2048;


enum FILE__STATE {
    FILE_FAILURE = -1,
    FILE__SUCCESS = 0,
};


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
    if(fd == FILE_FAILURE){
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
    if(fd == FILE_FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    write(fd, text, strlen(text));
    close(fd);
}


void append_file(char* filepath, char* text){
    int fd = open(filepath, O_CREAT|O_WRONLY|O_APPEND, 0600);
    if(fd == FILE_FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    write(fd, text, strlen(text));
    close(fd);
}