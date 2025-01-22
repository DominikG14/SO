#pragma once

#include "color.h"


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