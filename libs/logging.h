#pragma once

#include <stdio.h>

#include "color.h"


int NONE = -1;


enum WHO_ABBR {
    WHO__POOL_COMPLEX,
    WHO__CLIENT,
    WHO__LIFEGUARD,
    WHO__CHILD,
    WHO__CASHIER,
};
const char* WHO_NAME[] = {
    "POOL_COMPLEX",
    "CLIENT",
    "LIFEGUARD",
    "CHILD",
    "CASHIER",
};
void (*WHO_CLR[])() = {
    cyan,
    blue,
    red,
    green,
    yellow,
};


enum ACTION_ABBR {
    ACTION__ENTERED,
    ACTION__LEFT,

    ACTION__OPENED,
    ACTION__CLOSED,

    ACTION__PAID,
    ACTION__RECIVED_PAY,
};
const char* ACTION_NAME[] = {
    "ENTERED",
    "LEFT",

    "OPENED",
    "CLOSED",

    "PAID",
    "RECIVED_PAY",
};
void (*ACTION_CLR[])() = {
    green,
    red,

    green,
    red,

    yellow,
    yellow,
};


enum LOCATION_ABBR {
    LOCATION__POOL_COMPLEX,
    LOCATION__CASH_QUEUE,

    LOCATION__OLIMPIC_POOL,
    LOCATION__OLIMPIC_QUEUE,

    LOCATION__LEISURE_POOL,
    LOCATION__LEISURE_QUEUE,

    LOCATION__PADDLING_POOL,
    LOCATION__PADDLING_QUEUE,
};
const char* LOCATION_NAME[] = {
    "POOL_COMPLEX",
    "CASH_QUEUE",

    "OLIMPIC_POOL",
    "OLIMPIC_QUEUE",

    "LEISURE_POOL",
    "LEISURE_QUEUE",

    "PADDLING_POOL",
    "PADDLING_QUEUE",
};
void (*LOCATION_CLR[])() = {
    cyan,
    yellow,

    cyan,
    yellow,

    cyan,
    yellow,

    cyan,
    yellow,
};


enum REASON_ABBR {
    REASON__COMPLEX_CLOSED,
    REASON__POOL_CLOSED,
    REASON__WATER_CHANGE,
    REASON__END_OF_SWIM_TIME,

    REASON__CASH_CLOSED,
    REASON__PAYMENT_FINISHED,
};
const char* REASON_NAME[] = {
    "COMPLEX_CLOSED",
    "POOL_CLOSED",
    "WATER_CHANGE",
    "END_OF_SWIM_TIME",

    "CASH_CLOSED",
    "PAYMENT_FINISHED",
};
void (*REASON_CLR[])() = {
    cyan,
    red,
    blue,
    blue,

    red,
    yellow,
};


void log_console(int ID, int WHO, int ACTION, int LOCATION, int REASON){
    printf_clr(white, "%d\t", ID);
    printf_clr(WHO_CLR[WHO], "%s\t", WHO_NAME[WHO]);
    printf_clr(ACTION_CLR[ACTION], "%s\t", ACTION_NAME[ACTION]);
    printf_clr(LOCATION_CLR[LOCATION], "%s", LOCATION_NAME[LOCATION]);

    if(REASON == NONE){
        printf("\n");
        return;
    }

    printf_clr(REASON_CLR[REASON], "\t%s\n", REASON_NAME[REASON]);
}