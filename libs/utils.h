#pragma once


// Converts HH:MM time format to minutes
int time_HHMM(int hour, int min){
    return hour * 60 + min;
}