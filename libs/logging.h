#pragma once

#include <stdio.h>
#include <string.h>

#include "keys_id.h"
#include "low.h"

#include "logging/WHO.h"
#include "logging/ACTION.h"
#include "logging/LOCATION.h"
#include "logging/REASON.h"


// For loggin addition data afterwards
void __log_console(int ID, int WHO, int ACTION, int LOCATION, int REASON){
    printf_clr(WHO_CLR[WHO], "%-10d", ID);
    printf_clr(WHO_CLR[WHO], "%-18s", WHO_NAME[WHO]);
    printf_clr(ACTION_CLR[ACTION], "%-18s", ACTION_NAME[ACTION]);
    printf_clr(LOCATION_CLR[LOCATION], "%-20s", LOCATION_NAME[LOCATION]);

    if(REASON == REASON__NONE){
        return;
    }

    printf_clr(REASON_CLR[REASON], "%-20s", REASON_NAME[REASON]);
}


void log_console(int ID, int WHO, int ACTION, int LOCATION, int REASON){
    __log_console(ID, WHO, ACTION, LOCATION, REASON);
    printf("\n");
}


void log_console_with_data(int ID, int WHO, int ACTION, int LOCATION, int REASON, void (*data)()){
    __log_console(ID, WHO, ACTION, LOCATION, REASON);
    data();
    printf("\n");
}