#pragma once

#include "vars.h"
#include "bool.h"
#include "time.h"


void disp_curr_time(){
    if(open_complex_time()) disp_time(COMPLEX_OPEN_TIME);
    else if(close_complex_time()) disp_time(COMPLEX_CLOSE_TIME);
    else disp_time(TIME_CURR);
}


void increase_time(){
    sleep(1);
    TIME_CURR += TIME_PER_SEC;
}
