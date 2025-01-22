#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


const int FILE__SIZE = 2048;


enum FILE__STATE {
    FILE_FAILURE = -1,
    FILE__SUCCESS = 0,
};


char* alloc(){
    char* result =(char*) malloc(FILE__SIZE * sizeof(char));
    if(result == NULL){
        perror(__func__);
        exit(1);
    }

    return result;
}


char* read_file(const char* filepath, size_t* result_size){
    // File opening
    int fd = open(filepath, O_RDONLY);
    if(fd == FILE_FAILURE){
        perror(__func__);
        exit(EXIT_SUCCESS);
    }

    // Resulting text
    char* result = NULL;

    // Read file content
    size_t bytes_read;
    size_t curr_size;
    size_t total_bytes = 0;
    char buffer[FILE__SIZE];
    while((bytes_read = read(fd, buffer, FILE__SIZE)) > 0){

        if(result == NULL){
            result = alloc();
            curr_size = 1;
        } else {
            curr_size++;
            result =(char*) realloc(result, FILE__SIZE * curr_size * sizeof(char));
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
    int fd = open(filepath, O_CREAT|O_WRONLY|O_TRUNC, 0600);
    if(fd == FILE_FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    write(fd, text, strlen(text));
    close(fd);
}


size_t num_len(int num){
    size_t len = 0;
    while(num > 0){
        len++;
        num /= 10;
    }

    return len;
}


void save_id(char* filepath, int id){
    size_t size = num_len(id) + 1;

    char* id_str =(char*) malloc(size);
    snprintf(id_str, size, "%d", id);
    write_file(filepath, id_str);

    free(id_str), id_str = NULL;
}


int get_id(char* filepath){
    char* id_str = read_file(filepath, NULL);
    int id = atoi(id_str);

    free(id_str), id_str = NULL;
    return id;
}