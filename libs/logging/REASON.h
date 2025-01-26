#pragma once

#include "colors.h"


enum REASON_ABBR {
    REASON__NONE,

    REASON__COMPLEX_CLOSED,
    REASON__POOL_CLOSED,
    REASON__WATER_CHANGE,
    REASON__END_OF_SWIM_TIME,
    REASON__NOT_ENOUGH_SPACE,
    REASON__SPACE_AVAILABLE,

    REASON__CASH_CLOSED,
    REASON__PAYMENT_FINISHED,
};
const char* REASON_NAME[] = {
    "",

    "COMPLEX_CLOSED",
    "POOL_CLOSED",
    "WATER_CHANGE",
    "END_OF_SWIM_TIME",
    "NOT_ENOUGH_SPACE",
    "SPACE_AVAILABLE",

    "CASH_CLOSED",
    "PAYMENT_FINISHED",
};
void (*REASON_CLR[])() = {
    white,

    cyan,
    red,
    blue,
    blue,
    red,
    green,

    red,
    yellow,
};