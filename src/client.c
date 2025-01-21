#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "client.h"
#include "low/signal.h"


bool CLIENT__SWIM = true;

void leave_pool(){
    CLIENT__SWIM = false;
}


int main(){
    setup();

    Client client;

    client.age = rand_int(11, 70);
    client.swim_cap_on = rand_swim_cap();
    int child_age = rand_int(1, 10);

    printf("%d: swim_cap: %d, age %d", getpid(), client.swim_cap_on, client.age);
    if(rand_child(client.age, child_age)){
        printf(", child: %d", child_age);
    }
    printf("\n");

    handle_signal(SIG__CLOSE_POOL, leave_pool);
    while(CLIENT__SWIM){
        // TODO: Add Client functionality here
    }
    printf("%d: Client left the pool\n", getpid());

    return 0;
}