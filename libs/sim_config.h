#pragma once

#include <stdbool.h>

#include "low/files.h"
#include "time.h"


enum SIM_CONFIG_VARS {
    // Simulation time
    TIME__PER_SEC, // 1 real second = (TIME_PER_SEC) minutes in simulation 
    TIME__START,
    TIME__END,

    // Complex 
    COMPLEX__OPEN_TIME,
    COMPLEX__CLOSE_TIME,
    COMPLEX__WATER_CHANGE_PERC,
    COMPLEX__WATER_TIME_MIN,
    COMPLEX__WATER_TIME_MAX,

    // Client
    CLIENT__SPAWN_PERC,
    CLIENT__HAS_CHILD_PERC,
    CLIENT__SWIM_CAP_PREC,
    CLIENT_VIP_PERC,
    CLIENT__MIN_SWIM_TIME,
    CLIENT__MAX_SWIM_TIME,
    CLIENT__MIN_AGE,
    CLIENT__MAX_AGE,


    // Child
    CHILD__MIN_AGE,
    CHILD__MAX_AGE,
    CHILD__BABY_AGE,

    // Cashier
    CASHIER__MIN_PAYMENT_TIME,
    CASHIER__MAX_PAYMENT_TIME,

    // Pools
    POOL__OLIMPIC_MAX_SIZE,
    POOL__LEISURE_MAX_SIZE,
    POOL__PADDLING_MAX_SIZE,

    // Leisure avg
    POOL__LEISURE_AGE_AVG,

    // Lifeguard
    LIFEGUARD__CLOSE_POOL_PERC,
    LIFEGUARD__CLOSE_POOL_MIN_TIME,
    LIFEGUARD__CLOSE_POOL_MAX_TIME,
    
    SIM_CONFIG_SIZE,
};


int SIM_CONFIG[SIM_CONFIG_SIZE];
char* SIM_CONFIG_FILEPATH = "./config/olimpic";


// Loads simulation configuration from 'SIM_CONFIG_FILEPATH' into local 'SIM_CONFIG' array
void load_config(){
    size_t size; 
    char* config = read_file(SIM_CONFIG_FILEPATH, &size);

    bool START_READING = false;
    bool TIME_VAR = false;
    char* option = alloc();
    int option_i;
    int sim_i = 0;

    for(int i = 0; i <= size; i++){
        if(config[i] == '='){
            option_i = 0;
            START_READING = true;
            TIME_VAR = false;
            continue;
        }

        if(config[i] == ':'){
            TIME_VAR = true;
        }

        if(config[i] == '\n' || i == size){
            option[option_i] = '\0';
            if(TIME_VAR){
                bool HOUR_NOW = true;
                int hour, min, buf_i = 0;
                char buffer[3];

                for(int j = 0; j <= option_i; j++){
                    if(option[j] == ':' || j == option_i){
                        buffer[buf_i] = '\0';
                        if(HOUR_NOW) hour = atoi(buffer);
                        else min = atoi(buffer);

                        buf_i = 0;
                        HOUR_NOW = false;
                        continue;
                    }
                    buffer[buf_i] = option[j];
                    buf_i++;
                }

                SIM_CONFIG[sim_i] = time_HHMM(hour, min);
            }
            else SIM_CONFIG[sim_i] = atoi(option);

            sim_i++;
            START_READING = false;
        }

        option[option_i] = config[i];
        option_i++;
    }

    free(config), config = NULL;
}