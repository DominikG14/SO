#pragma once

#include "color.h"


// Converts HH:MM time format to minutes
int time_HHMM(int hour, int min){
    return hour * 60 + min;
}


// Display time in HH:MM format
void disp_time(int time){
    int hour = time / 60;
    int min = time % 60;

    bold_underline_cyan();
    printf("\n");
    if(hour < 10){
        printf("0");
    }
    printf("%d:", hour);

    if(min < 10){
        printf("0");
    }
    printf("%d", min);
    reset();
    printf("\n");
}