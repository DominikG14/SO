#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "random.h"


struct Client {
    int age;
    bool swim_cap_on;
} typedef Client;


void setup(){
    srand(getpid());
}


const int HAS_CHILD_PERC = 29;
const int SWIM_CAP_PREC = 30;


bool rand_child(int client_age, int child_age){
    int child_perc = rand_int(1, 100);
    if(client_age - child_age >= 18 && child_perc <= HAS_CHILD_PERC){
        return true;
    }

    return false;
}


bool rand_swim_cap(){
    int swim_cap_perc = rand_int(1, 100);
    if(swim_cap_perc <= SWIM_CAP_PREC){
        return true;
    }

    return false;
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

    return 0;
}