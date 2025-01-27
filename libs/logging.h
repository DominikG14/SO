#pragma once

#include "logging/WHO.h"
#include "logging/ACTION.h"
#include "logging/LOCATION.h"
#include "logging/REASON.h"

#include "low.h"
#include "global.h"
#include "config.h"


int LOG__DONT_CHANGE = -1; // For leaving current info as it is


// For loggin additional data afterwards
void __log_console(int WHO, int ACTION, int LOCATION, int REASON){
    printf_clr(WHO_CLR[WHO], "%-10d", getpid());
    printf_clr(WHO_CLR[WHO], "%-18s", WHO_NAME[WHO]);
    printf_clr(ACTION_CLR[ACTION], "%-18s", ACTION_NAME[ACTION]);
    printf_clr(LOCATION_CLR[LOCATION], "%-20s", LOCATION_NAME[LOCATION]);
    printf_clr(REASON_CLR[REASON], "%-20s", REASON_NAME[REASON]);
}


void log_file(char* filepath, int WHO, int ACTION, int LOCATION, int REASON, char* data){
    char buffer[FILE_SIZE];
    sprintf(buffer, "%-10d%-18s%-18s%-20s%-20s%s\n",
        getpid(),
        WHO_NAME[WHO],
        ACTION_NAME[ACTION],
        LOCATION_NAME[LOCATION],
        REASON_NAME[REASON],
        data
    );

    append_file(filepath, buffer);
}


void log_console(int WHO, int ACTION, int LOCATION, int REASON){
    __log_console(WHO, ACTION, LOCATION, REASON);
    log_file(LOGGING_FILEPATH, WHO, ACTION, LOCATION, REASON, "");
    printf("\n");
}


void log_console_with_data(int WHO, int ACTION, int LOCATION, int REASON, char* (*data)()){
    __log_console(WHO, ACTION, LOCATION, REASON);
    char* return_data = data();
    printf("\n");

    log_file(LOGGING_FILEPATH, WHO, ACTION, LOCATION, REASON, return_data);
    free(return_data), return_data = NULL;
}