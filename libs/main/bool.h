#pragma once

#include <stdbool.h>

#include "vars.h"


bool open_complex_time(){
    return !COMPLEX_IS_OPEN && TIME_CURR >= COMPLEX_OPEN_TIME && TIME_CURR <= COMPLEX_CLOSE_TIME;
}


bool close_complex_time(){
    return COMPLEX_IS_OPEN && TIME_CURR > COMPLEX_CLOSE_TIME;
}