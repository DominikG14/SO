#pragma once

#include "color.h"


enum REASON_ABBR {
    REASON__NONE = -1,
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