#include "main.h"


char* data;


int main() {
    setup();

    while(TIME_CURR <= TIME_END){
        disp_curr_time();

        while(get_msq_count(LOGGER_MSQID) != 0){
            data = get_msq(LOGGER_MSQID, 1);
            printf("\t%s\n", data);
            free(data), data = NULL;
        }

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