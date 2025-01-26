#pragma once

#include "colors.h"


enum LOCATION_ABBR {
    LOCATION__OLIMPIC_POOL,
    LOCATION__LEISURE_POOL,
    LOCATION__PADDLING_POOL,

    LOCATION__OLIMPIC_QUEUE,
    LOCATION__LEISURE_QUEUE,
    LOCATION__PADDLING_QUEUE,

    LOCATION__POOL_COMPLEX,
    LOCATION__CASH_QUEUE,
};
const char* LOCATION_NAME[] = {
    "OLIMPIC_POOL",
    "LEISURE_POOL",
    "PADDLING_POOL",

    "OLIMPIC_QUEUE",
    "LEISURE_QUEUE",
    "PADDLING_QUEUE",

    "POOL_COMPLEX",
    "CASH_QUEUE",
};
void (*LOCATION_CLR[])() = {
    cyan,
    cyan,
    cyan,

    yellow,
    yellow,
    yellow,
    
    cyan,
    yellow,
};