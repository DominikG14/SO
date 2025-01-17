#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>


enum KEY_STATES {
    KEY_SUCCESS = 0,
    KEY_FAILURE = -1,
};


const char* KEY_PATH = ".";


key_t get_key(int id){
    key_t key = ftok(KEY_PATH, id);
    if(key == KEY_FAILURE){
        perror(__func__);
        exit(EXIT_FAILURE);
    }

    return key;
}