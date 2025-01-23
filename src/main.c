#include "main.h"


int main() {
    setup();

    while(TIME_CURR <= TIME_END){
        disp_curr_time();

        if(open_complex_time()){
            open_complex();
            continue;
        }

        if(close_complex_time()){
            close_complex();
            continue;
        }

        let_clients_in();
        increase_time();
    }

    clean_up();
    return 0;
}