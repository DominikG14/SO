#include "client.h"


int main(){
    setup();

    spawn_client();
    wait_in_cash_queue();
    choose_pool();

    return 0;
}