#pragma once

#include "low.h"
#include "global.h"
#include "colors.h"
#include "random.h"
#include "pools.h"


// -------------------- Struct --------------------
struct Client {
    int age;
    bool swim_cap_on;
} typedef Client;


struct Child {
    pthread_t tid;

    int age;
    bool swim_cap_on;
    bool diaper_on;
} typedef Child;


// -------------------- Config --------------------
int CLIENT_SWIM_CAP_PREC = 30;
int CLIENT_HAS_CHILD_PERC = 50;
int CLIENT_MIN_AGE = 10;
int CLIENT_MAX_AGE = 70;
int CLIENT_MIN_SWIM_TIME = 1;
int CLIENT_MAX_SWIM_TIME = 3;

int CHILD_MIN_AGE = 1;
int CHILD_MAX_AGE = 9;
int CHILD_BABY_AGE = 5;
int CHILD_DIAPER_AGE = 3;


// -------------------- Local --------------------
Client client;
Child child;


// -------------------- Logging --------------------
// void log_client(int WHO){
//     log_console(
//         WHO,
//         CLIENT_LOCATION,
//         CLIENT_ACTION,
//         CLIENT_REASON
//     );
// }


// void log_client_with_data(int WHO, char* (*data)()){
//     log_console_with_data(
//         WHO,
//         CLIENT_LOCATION,
//         CLIENT_ACTION,
//         CLIENT_REASON,
//         data
//     );
// }


// -------------------- Random generation --------------------
bool rand_child(int client_age, int child_age){
    int child_perc = rand_int(1, 100);
    if(client_age - child_age >= 18 && child_perc <= CLIENT_HAS_CHILD_PERC){
        return true;
    }

    return false;
}


bool rand_swim_cap(){
    int swim_cap_perc = rand_int(1, 100);
    if(swim_cap_perc <= CLIENT_SWIM_CAP_PREC){
        return true;
    }

    return false;
}


int rand_swim_time(){
    return rand_int(CLIENT_MIN_SWIM_TIME, CLIENT_MAX_SWIM_TIME);
}


// -------------------- Bool checks --------------------
bool client_is_underage(){
    return client.age < 18; 
}


bool client_has_child(){
    return child.tid != -1;
}


bool client_has_baby(){
    return client_has_child() && child.age <= CHILD_BABY_AGE;
}


// -------------------- Child Functionality --------------------
char* child_get_data(){
    // Child Age
    printf_clr(green, "| ");
    printf("age: %d", child.age);

    // Child swim cap
    char swim_cap[4];

    printf_clr(green, " | ");
    printf("swim_cap_on: ");
    if(child.swim_cap_on) printf_clr(green, "yes"), strcpy(swim_cap, "yes");
    else printf_clr(red, "no"), strcpy(swim_cap, "no");

    // Child diaper
    char diaper[4];

    printf_clr(green, " | ");
    printf("diaper_on: ");
    if(child.diaper_on) printf_clr(green, "yes"), strcpy(diaper, "yes");
    else printf_clr(red, "no"), strcpy(diaper, "no");

    // End
    printf_clr(green, " |");


    // log to file
    char* data =(char*) malloc(FILE_SIZE);
    sprintf(data, "| age: %d | swim_cap_on: %s | diaper_on: %s |", child.age, swim_cap, diaper);

    return data;
}


void* child_enter_cash_queue(void * thread_flags){
    printf_clr(green, "%d: dziecko weszlo do kolejki\n", getpid());

    pthread_exit(EXIT_SUCCESS);
}


void* child_leave_cash_queue(void * thread_flags){
    printf_clr(green, "%d: dziecko opuscilo kolejke\n", getpid());

    pthread_exit(EXIT_SUCCESS);
}


void* child_leave_pool(void* thread_flags){
    switch(CLIENT_LOCATION){
        case LOCATION__CASH_QUEUE:
            printf_clr(green, "%d: dziecko opuscilo kase - zamkniecie kompleksu\n", getpid());
            break;
        
        case LOCATION__OLIMPIC_POOL:
            printf_clr(green, "%d: dziecko opuscilo basen olimpijski\n", getpid());
            break;
        
        case LOCATION__LEISURE_POOL:
            printf_clr(green, "%d: dziecko opuscilo basen rekreacyjny\n", getpid());
            break;

        case LOCATION__PADDLING_POOL:
            printf_clr(green, "%d: dziecko opuscilo brodzik\n", getpid());
            break;
    }
}


void* child_leave_complex(void * thread_flags){
    printf_clr(green, "%d: dziecko opuscilo kompleks\n", getpid());
    pthread_exit(EXIT_SUCCESS);
}


// -------------------- Client Functionality --------------------
char* client_get_data(){
    // Client Age
    printf_clr(blue, "| ");
    printf("age: %d", client.age);

    // Client swim cap
    char swim_cap[4];

    printf_clr(blue, " | ");
    printf("swim_cap_on: ");
    if(client.swim_cap_on) printf_clr(green, "yes"), strcpy(swim_cap, "yes");
    else printf_clr(red, "no"), strcpy(swim_cap, "no");

    // Client child
    char has_child[4];

    printf_clr(blue, " | ");
    printf("with_child: ");
    if(client_has_child()) printf_clr(green, "yes"), strcpy(has_child, "yes");
    else printf_clr(red, "no"), strcpy(has_child, "no");

    // End
    printf_clr(blue, " |");

    // log to file
    char* data =(char*) malloc(FILE_SIZE);
    sprintf(data, "| age: %d | swim_cap_on: %s | with_child: %s |", client.age, swim_cap, has_child);

    return data;
}

void client_set_data(){
    // Set startup logging
    CLIENT_LOCATION=LOCATION__CASH_QUEUE;
    CLIENT_ACTION=ACTION__ENTERED;
    CLIENT_REASON=REASON__NONE;

    // Set child status
    child.tid = -1;

    // Set client data
    client.age = rand_int(CLIENT_MIN_AGE, CLIENT_MAX_AGE);
    client.swim_cap_on = rand_swim_cap();
    int child_age = rand_int(CHILD_MIN_AGE, CHILD_MAX_AGE);

    if(rand_child(client.age, child_age)){
        child.age = child_age;
        child.swim_cap_on = rand_swim_cap();

        if(child.age <= CHILD_DIAPER_AGE){
            child.diaper_on = true;
        }

        child.tid = new_thread(child_enter_cash_queue, NULL);
        pthread_join(child.tid, NULL);
    }

    printf_clr(blue, "%d: klient wszedl do kolejki\n", getpid());
}


void client_leave_pool(){
    switch(CLIENT_LOCATION){
        case LOCATION__CASH_QUEUE:
            printf_clr(blue, "%d: klient opuscil kase - zamkniecie kompleksu\n", getpid());
            break;
        
        case LOCATION__OLIMPIC_POOL:
            printf_clr(blue, "%d: klient opuscil basen olimpijski\n", getpid());
            break;
        
        case LOCATION__LEISURE_POOL:
            printf_clr(blue, "%d: klient opuscil basen rekreacyjny\n", getpid());
            break;

        case LOCATION__PADDLING_POOL:
            printf_clr(blue, "%d: klient opuscil brodzik\n", getpid());
            break;
    }
}


void client_leave_complex(){
    client_leave_pool();

    if(client_has_child()){
        child.tid = new_thread(child_leave_pool, NULL);
        pthread_join(child.tid, NULL);

        child.tid = new_thread(child_leave_complex, NULL);
        pthread_join(child.tid, NULL);
    }

    printf_clr(blue, "%d: klient opuscil kompleks basenow\n", getpid());   
    exit(EXIT_SUCCESS);
}


void client_choose_pool(){
    if(client_has_baby()){
        paddling_join_pool();
    }

    if(client_is_underage() || client_has_child()){
        leisure_join_pool();
    }


    switch(rand_int(OLIMPIC, LEISURE)){
        case OLIMPIC:
            olimpic_join_pool();

        case LEISURE:
            leisure_join_pool();
    }
}


// -------------------- IPCS --------------------
void __close_complex_handler(){
    client_leave_complex();
}


void __set_close_complex_handler(){
    struct sigaction handler;
    handler.sa_handler = __close_complex_handler;
    sigemptyset(&handler.sa_mask);
    handler.sa_flags = 0;
    sigaction(SIG_CLOSE_COMPLEX, &handler, 0);
}


void __access_kasa_msq(){
    key_t key = get_key(KEY_CASH_MSQ);
    if((CASH_MSQID = msgget(key, 0600)) == FAILURE){
        perror("klient - msgget");
        exit(EXIT_FAILURE);
    }
}


void setup(){
    srand(getpid());
    __set_close_complex_handler();
    __access_kasa_msq();
    client_set_data();
}