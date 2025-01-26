#include "main.h"
#include "logging.h"


int main(){
    setup();

    for(TIME_CURR = TIME_START; TIME_CURR <= TIME_END; TIME_CURR += TIME_PER_SEC){
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

        imitate_time(1);
    }

    clean_up();
    exit(EXIT_SUCCESS);
}