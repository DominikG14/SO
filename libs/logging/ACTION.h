#pragma once

#include "color.h"


enum ACTION_ABBR {
    ACTION__ENTERED,
    ACTION__LEFT,

    ACTION__OPENED,
    ACTION__CLOSED,

    ACTION__PAID,
    ACTION__RECIVED_PAY,
    ACTION__RECIVED_BILL,
};
const char* ACTION_NAME[] = {
    "ENTERED",
    "LEFT",

    "OPENED",
    "CLOSED",

    "PAID",
    "RECIVED_PAY",
    "RECIVED_BILL",
};
void (*ACTION_CLR[])() = {
    green,
    red,

    green,
    red,

    yellow,
    yellow,
    yellow,
};