#pragma once

#include "colors.h"


enum REASON_ABBR {
    REASON__NONE,

    REASON__COMPLEX_CLOSED,
    REASON__POOL_CLOSED,
    REASON__POOL_OPENED,
    REASON__END_OF_SWIM_TIME,
    REASON__NOT_ENOUGH_SPACE,
    REASON__SPACE_AVAILABLE,
    REASON__AGE_ABOVE_AVG,
    REASON__AGE_BELOW_AVG,

    REASON__CASH_CLOSED,
    REASON__PAYMENT_FINISHED,
};
const char* REASON_NAME[] = {
    "",

    "COMPLEX_CLOSED",
    "POOL_CLOSED",
    "POOL_OPENED",
    "END_OF_SWIM_TIME",
    "NOT_ENOUGH_SPACE",
    "SPACE_AVAILABLE",
    "AGE_ABOVE_AVG",
    "AGE_BELOW_AVG",

    "CASH_CLOSED",
    "PAYMENT_FINISHED",
};
void (*REASON_CLR[])() = {
    white,

    cyan,
    red,
    green,
    blue,
    red,
    green,
    magenta,
    magenta,

    red,
    yellow,
};