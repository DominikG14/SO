#pragma once

#include <stdbool.h>

#include "low.h"
#include "colors.h"


// -------------------- Paths --------------------
char* CONFIG_FILEPATH = "./config/config";
char* LOGGING_FILEPATH = "./logs";


// -------------------- Local config vars --------------------
// Time
int TIME_PER_SEC;
int TIME_START;
int TIME_END;
int TIME_DISP;

// Complex 
int COMPLEX_OPEN_TIME;
int COMPLEX_CLOSE_TIME;
int COMPLEX_WATER_TIME_MIN;
int COMPLEX_WATER_TIME_MAX;

// Client
int CLIENT_SPAWN_PERC;
int CLIENT_HAS_CHILD_PERC;
int CLIENT_SWIM_CAP_PREC;
int CLIENT_VIP_PERC;
int CLIENT_MIN_SWIM_TIME;
int CLIENT_MAX_SWIM_TIME;
int CLIENT_MIN_AGE;
int CLIENT_MAX_AGE;

// Child
int CHILD_MIN_AGE;
int CHILD_MAX_AGE;
int CHILD_BABY_AGE;
int CHILD_DIAPER_AGE;

// Cashier
int CASHIER_MIN_PAYMENT_TIME;
int CASHIER_MAX_PAYMENT_TIME;

// Pools
int POOL_OLIMPIC_MAX_SIZE;
int POOL_LEISURE_MAX_SIZE;
int POOL_PADDLING_MAX_SIZE;

// Leisure avg
int POOL_LEISURE_AGE_AVG;

// Lifeguard
int LIFEGUARD_CLOSE_POOL_PERC;
int LIFEGUARD_CLOSE_POOL_MIN_TIME;
int LIFEGUARD_CLOSE_POOL_MAX_TIME;

// Sleep
int SLEEP_ACTIVE;


// -------------------- Funcs --------------------
enum CONFIG_VARS {
    // Simulation time
    TIME__PER_SEC, // 1 real second = (TIME_PER_SEC) minutes in simulation 
    TIME__START,
    TIME__END,
    TIME__DISP,

    // Complex 
    COMPLEX__OPEN_TIME,
    COMPLEX__CLOSE_TIME,

    // Client
    CLIENT__SPAWN_PERC,
    CLIENT__HAS_CHILD_PERC,
    CLIENT__SWIM_CAP_PREC,
    CLIENT__VIP_PERC,
    CLIENT__MIN_SWIM_TIME,
    CLIENT__MAX_SWIM_TIME,
    CLIENT__MIN_AGE,
    CLIENT__MAX_AGE,


    // Child
    CHILD__MIN_AGE,
    CHILD__MAX_AGE,
    CHILD__BABY_AGE,
    CHILD__DIAPER_AGE,

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

    // Sleep
    SLEEP__ACTIVE,
    
    CONFIG_SIZE,
};


int CONFIG[CONFIG_SIZE];


// Converts HH:MM time format to minutes
int time_HHMM(int hour, int min){
    return hour * 60 + min;
}


// Loads simulation configuration from 'CONFIG_FILEPATH' into local 'CONFIG' array
void load_config(){
    size_t size; 
    char* config = read_file(CONFIG_FILEPATH, &size);

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

                CONFIG[sim_i] = time_HHMM(hour, min);
            }
            else CONFIG[sim_i] = atoi(option);

            sim_i++;
            START_READING = false;
        }

        option[option_i] = config[i];
        option_i++;
    }

    free(config), config = NULL;
}


void set_config_vars(){
    load_config();

    // Time
    TIME_PER_SEC = CONFIG[TIME__PER_SEC];
    TIME_START = CONFIG[TIME__START];
    TIME_END = CONFIG[TIME__END];
    TIME_DISP = CONFIG[TIME__DISP];

    // Complex 
    COMPLEX_OPEN_TIME = CONFIG[COMPLEX__OPEN_TIME];
    COMPLEX_CLOSE_TIME = CONFIG[COMPLEX__CLOSE_TIME];

    // Client
    CLIENT_SPAWN_PERC = CONFIG[CLIENT__SPAWN_PERC];
    CLIENT_HAS_CHILD_PERC = CONFIG[CLIENT__HAS_CHILD_PERC];
    CLIENT_SWIM_CAP_PREC = CONFIG[CLIENT__SWIM_CAP_PREC];
    CLIENT_VIP_PERC = CONFIG[CLIENT__VIP_PERC];
    CLIENT_MIN_SWIM_TIME = CONFIG[CLIENT__MIN_SWIM_TIME] / TIME_PER_SEC;
    CLIENT_MAX_SWIM_TIME = CONFIG[CLIENT__MAX_SWIM_TIME] / TIME_PER_SEC;
    CLIENT_MIN_AGE = CONFIG[CLIENT__MIN_AGE];
    CLIENT_MAX_AGE = CONFIG[CLIENT__MAX_AGE];


    // Child
    CHILD_MIN_AGE = CONFIG[CHILD__MIN_AGE];
    CHILD_MAX_AGE = CONFIG[CHILD__MAX_AGE];
    CHILD_BABY_AGE = CONFIG[CHILD__BABY_AGE];
    CHILD_DIAPER_AGE = CONFIG[CHILD__DIAPER_AGE];

    // Cashier
    CASHIER_MIN_PAYMENT_TIME = CONFIG[CASHIER__MIN_PAYMENT_TIME] / TIME_PER_SEC;
    CASHIER_MAX_PAYMENT_TIME = CONFIG[CASHIER__MAX_PAYMENT_TIME] / TIME_PER_SEC;

    // Pools
    POOL_OLIMPIC_MAX_SIZE = CONFIG[POOL__OLIMPIC_MAX_SIZE];
    POOL_LEISURE_MAX_SIZE = CONFIG[POOL__LEISURE_MAX_SIZE];
    POOL_PADDLING_MAX_SIZE = CONFIG[POOL__PADDLING_MAX_SIZE];

    // Leisure avg
    POOL_LEISURE_AGE_AVG = CONFIG[POOL__LEISURE_AGE_AVG];

    // Lifeguard
    LIFEGUARD_CLOSE_POOL_PERC = CONFIG[LIFEGUARD__CLOSE_POOL_PERC];
    LIFEGUARD_CLOSE_POOL_MIN_TIME = CONFIG[LIFEGUARD__CLOSE_POOL_MIN_TIME] / TIME_PER_SEC;
    LIFEGUARD_CLOSE_POOL_MAX_TIME = CONFIG[LIFEGUARD__CLOSE_POOL_MAX_TIME] / TIME_PER_SEC;

    // Sleep
    SLEEP_ACTIVE = CONFIG[SLEEP__ACTIVE];
}


// -------------------- Error types --------------------
int ERROR_COUNT = 0;

void __check_percentage(int value, char* name){
    if(! (value >= 0 && value <= 100) ){
        printf_clr(bold_cyan, "%s", name);
        printf_clr(red,  " must be a perecentage (");
        printf_clr(magenta,  "%%");
        printf_clr(red,  ")\n");
        printf_clr(red,  "from ");
        printf_clr(blue,  "0");
        printf_clr(red,  " to ");
        printf_clr(blue,  "100\n\n");
        ERROR_COUNT++;
    }
}


void __check_positive(int value, char* name){
    if( !(value > 0) ){
        printf_clr(bold_cyan, "%s", name);
        printf_clr(red,  " must be positive (");
        printf_clr(green,  "+");
        printf_clr(red,  ")\n\n");
        ERROR_COUNT++;
    }
}


void __check_HHMM(int value, char* name){
    if( !(value >= time_HHMM(0, 0) && value <= time_HHMM(23, 59)) ){
        printf_clr(bold_cyan, "%s", name);
        printf_clr(red,  " must be in the proper time format (");
        printf_clr(blue,  "HH:MM");
        printf_clr(red,  ")\n");
        printf_clr(red,  "from ");
        printf_clr(blue,  "00:00");
        printf_clr(red,  " to ");
        printf_clr(blue,  "23:59\n\n");
        ERROR_COUNT++;
    }
}

void __check_nonnegative(int value, char* name){
    if( !(value >= 0) ){
        printf_clr(bold_cyan, "%s", name);
        printf_clr(red, " must be non negative (");
        printf_clr(green, "+");
        printf_clr(yellow, "0");
        printf_clr(red, ")\n\n");
        ERROR_COUNT++;
    }
}


void __check_chronology(int time_early, int time_later, char* name_early, char* name_later){
    if( !(time_early <= time_later) ){
        printf_clr(bold_cyan, "%s", name_early);
        printf_clr(red,  " can not be later than ");
        printf_clr(bold_cyan, "%s\n\n", name_later);
        ERROR_COUNT++;
    }
}

void __check_ge(int greater, int lesser, char* name_greater, char* name_lesser){
    if( !(greater >= lesser) ){
        printf_clr(bold_cyan, "%s", name_greater);
        printf_clr(red, " must be greater or equal to ");
        printf_clr(bold_cyan, "%s\n\n", name_lesser);
        ERROR_COUNT++;
    }
}


// Check
void check_config(){ // This is runed only once in 'main'
    __check_positive(TIME_PER_SEC, "TIME__PER_SEC");
    __check_HHMM(TIME_START, "TIME__START");
    __check_HHMM(TIME_END, "TIME__END");
    __check_chronology(TIME_START, TIME_END, "TIME__START", "TIME__END");

    __check_HHMM(COMPLEX_OPEN_TIME, "COMPLEX__OPEN_TIME");
    __check_HHMM(COMPLEX_CLOSE_TIME, "COMPLEX__END_TIME");
    __check_chronology(COMPLEX_OPEN_TIME, COMPLEX_CLOSE_TIME, "COMPLEX__OPEN_TIME", "COMPLEX__CLOSE_TIME");

    __check_percentage(CLIENT_SPAWN_PERC, "CLIENT__SPAWN_PERC");
    __check_percentage(CLIENT_HAS_CHILD_PERC, "CLIENT__HAS_CHILD_PERC");
    __check_percentage(CLIENT_SWIM_CAP_PREC, "CLIENT__SWIM_CAP_PREC");
    __check_percentage(CLIENT_VIP_PERC, "CLIENT__VIP_PERC");

    __check_nonnegative(CLIENT_MIN_SWIM_TIME, "CLIENT__MIN_SWIM_TIME");
    __check_nonnegative(CLIENT_MAX_SWIM_TIME, "CLIENT__MAX_SWIM_TIME");

    __check_positive(CLIENT_MIN_AGE, "CLIENT__MAX_AGE");
    __check_positive(CLIENT_MAX_AGE, "CLIENT__MAX_AGE");

    __check_positive(CHILD_MIN_AGE, "CHILD__MAX_AGE");
    __check_positive(CHILD_MAX_AGE, "CHILD__MAX_AGE");

    __check_ge(CLIENT_MIN_AGE, CHILD_MAX_AGE, "CLIENT__MIN_AGE", "CHILD__MAX_AGE");

    __check_positive(CHILD_BABY_AGE, "CHILD__BABY_AGE");
    __check_positive(CHILD_DIAPER_AGE, "CHILD__DIAPER_AGE");

    __check_nonnegative(CASHIER_MIN_PAYMENT_TIME, "CASHIER__MIN_PAYMENT_TIME");
    __check_nonnegative(CASHIER_MAX_PAYMENT_TIME, "CASHIER__MAX_PAYMENT_TIME");
    
    __check_nonnegative(POOL_OLIMPIC_MAX_SIZE, "POOL__OLIMPIC_MAX_SIZE");
    __check_nonnegative(POOL_LEISURE_MAX_SIZE, "POOL__LEISURE_MAX_SIZE");
    __check_nonnegative(POOL_PADDLING_MAX_SIZE, "POOL__PADDLING_MAX_SIZE");

    __check_ge(CLIENT_MAX_SWIM_TIME, CLIENT_MIN_SWIM_TIME, "CLIENT__MAX_SWIM_TIME", "CLIENT__MIN_SWIM_TIME");
    __check_ge(CLIENT_MAX_AGE, CLIENT_MIN_AGE, "CLIENT__MAX_AGE", "CLIENT__MIN_AGE");
    __check_ge(CHILD_MAX_AGE, CHILD_MIN_AGE, "CHILD__MAX_AGE", "CHILD__MIN_AGE");
    __check_ge(CASHIER_MAX_PAYMENT_TIME, CASHIER_MIN_PAYMENT_TIME, "CASHIER__MAX_PAYMENT_TIME", "CASHIER__MIN_PAYMENT_TIME");
    __check_ge(CLIENT_MAX_SWIM_TIME, CLIENT_MIN_SWIM_TIME, "CLIENT__MAX_SWIM_TIME", "CLIENT__MIN_SWIM_TIME");

    __check_nonnegative(POOL_LEISURE_AGE_AVG, "POOL__LEISURE_AGE_AVG");
    __check_percentage(LIFEGUARD_CLOSE_POOL_PERC, "LIFEGUARD__CLOSE_POOL_PERC");
    __check_nonnegative(LIFEGUARD_CLOSE_POOL_MIN_TIME, "LIFEGUARD__CLOSE_POOL_MIN_TIME");
    __check_nonnegative(LIFEGUARD_CLOSE_POOL_MAX_TIME, "LIFEGUARD__CLOSE_POOL_MAX_TIME");
    __check_ge(LIFEGUARD_CLOSE_POOL_MAX_TIME, LIFEGUARD_CLOSE_POOL_MIN_TIME, "LIFEGUARD__CLOSE_POOL_MAX_TIME", "LIFEGUARD__CLOSE_POOL_MIN_TIME");


    if(ERROR_COUNT != 0){
        printf_clr(bold_red, "ERROR_COUNT: %d\n", ERROR_COUNT);
        exit(EXIT_FAILURE);
    }
}