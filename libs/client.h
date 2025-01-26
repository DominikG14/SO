#pragma once

#include "low.h"
#include "global.h"
#include "colors.h"
#include "random.h"
#include "logging.h"


// -------------------- Func declaration --------------------
void log_client(int WHO);
void log_client_with_data(int WHO);
void set_client_info(int ACTION, int LOCATION, int REASON);

void olimpic_join_pool();
void leisure_join_pool();
void paddling_join_pool();

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
int CLIENT_HAS_CHILD_PERC = 0;
int CLIENT_MIN_AGE = 30;
int CLIENT_MAX_AGE = 70;
int CLIENT_MIN_SWIM_TIME = 1;
int CLIENT_MAX_SWIM_TIME = 3;

int CHILD_MIN_AGE = 1;
int CHILD_MAX_AGE = 9;
int CHILD_BABY_AGE = 5;
int CHILD_DIAPER_AGE = 3;


// -------------------- Local --------------------
int CLIENT_ACTION;
int CLIENT_LOCATION;
int CLIENT_REASON;

Client client;
Child child;


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


void* child_enter_complex(void* thread_flags){
    log_client_with_data(WHO__CHILD);
    pthread_exit(EXIT_SUCCESS);
}


void* child_keep_eye_on(void* thread_flags){
    log_client(WHO__CHILD);
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
    set_client_info(ACTION__ENTERED, LOCATION__POOL_COMPLEX, REASON__NONE);

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

        child.tid = new_thread(child_enter_complex, NULL);
        pthread_join(child.tid, NULL);
    }

    log_client_with_data(WHO__CLIENT);
}


void client_leave_complex(){
    set_client_info(ACTION__LEFT, LOG__DONT_CHANGE, REASON__COMPLEX_CLOSED), log_client(WHO__CLIENT);

    if(client_has_child()){
        child.tid = new_thread(child_keep_eye_on, NULL);
        pthread_join(child.tid, NULL);
    }

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


// -------------------- Logging --------------------
void log_client(int WHO){
    log_console(
        WHO,
        CLIENT_ACTION,
        CLIENT_LOCATION,
        CLIENT_REASON
    );
}


void log_client_with_data(int WHO){
    char* (*data)() = NULL;

    switch(WHO){
        case WHO__CLIENT:
            data = client_get_data;
            break;
        
        case WHO__CHILD:
            data = child_get_data;
            break;
    }

    log_console_with_data(
        WHO,
        CLIENT_ACTION,
        CLIENT_LOCATION,
        CLIENT_REASON,
        data
    );
}


void set_client_info(int ACTION, int LOCATION, int REASON){
    if(ACTION != LOG__DONT_CHANGE) CLIENT_ACTION = ACTION;
    if(LOCATION != LOG__DONT_CHANGE) CLIENT_LOCATION = LOCATION;
    if(REASON != LOG__DONT_CHANGE) CLIENT_REASON = REASON;
}


// -------------------- Olimpic pool --------------------
char* olimpic_get_data(int status){
    // Client age
    printf_clr(cyan, "| ");
    printf("client_age: %d", client.age);

    // Pool size
    printf_clr(cyan, " | ");
    switch(status){
        case STATUS_ENTER:
            printf("size: %d/%d (prev: %d/%d)", POOL_SIZE, POOL_OLIMPIC_MAX_SIZE, POOL_SIZE - 1, POOL_OLIMPIC_MAX_SIZE);
            break;

        case STATUS_LEAVE:
            printf("size: %d/%d (prev: %d/%d)", POOL_SIZE, POOL_OLIMPIC_MAX_SIZE, POOL_SIZE + 1, POOL_OLIMPIC_MAX_SIZE);
            break;

        case STATUS_NONE:
            printf("size: %d/%d", POOL_SIZE, POOL_OLIMPIC_MAX_SIZE);
    }
    printf_clr(cyan, " |");


    // log to file
    char* data =(char*) malloc(FILE_SIZE);
    switch(status){
        case STATUS_ENTER:
            sprintf(data, "| client_age: %d | size: %d/%d (prev: %d/%d) |", client.age, POOL_SIZE, POOL_OLIMPIC_MAX_SIZE, POOL_SIZE - 1, POOL_OLIMPIC_MAX_SIZE);
            break;

        case STATUS_LEAVE:
            sprintf(data, "| client_age: %d | size: %d/%d (prev: %d/%d) |", client.age, POOL_SIZE, POOL_OLIMPIC_MAX_SIZE, POOL_SIZE + 1, POOL_OLIMPIC_MAX_SIZE);
            break;

        case STATUS_NONE:
            sprintf(data, "| client_age: %d | size: %d/%d |", client.age, POOL_SIZE, POOL_OLIMPIC_MAX_SIZE);
    }

    return data;
}


void log_pool_data(char* (*data)(), int status){
    __log_console(
        WHO__CLIENT, // No childs allowed in Olimpic pool!
        CLIENT_ACTION,
        CLIENT_LOCATION,
        CLIENT_REASON
    );
    char* return_data = data(status);
    printf("\n");

    log_file(LOGGING_FILEPATH,
        WHO__CLIENT,
        CLIENT_ACTION, 
        CLIENT_LOCATION, 
        CLIENT_REASON, 
        return_data
    );

    free(return_data), return_data = NULL;
}


void olimpic_access_pool(){
    SEM_OPERATE.sem_flg = 0;
    SEM_OPERATE.sem_num = SEM_POOL_SHM;

    key_t key_msq, key_sem, key_shm;

    key_msq = get_key(KEY_OLIMPIC_POOL_MSQ);
    key_sem = get_key(KEY_OLIMPIC_POOL_SEM);
    key_shm = get_key(KEY_OLIMPIC_POOL_SHM);

    if((OLIMPIC_POOL_MSQID = msgget(key_msq, 0600)) == FAILURE){
        perror("klient - msgget - pools");
        exit(EXIT_FAILURE);
    }

    if((OLIMPIC_POOL_SEMID = semget(key_sem, SEM_POOL_NUM, 0600)) == FAILURE){
        perror("klient - semget - pools");
        exit(EXIT_FAILURE);
    }

    if((OLIMPIC_POOL_SHMID = shmget(key_shm, sizeof(int), 0600)) == FAILURE){
        perror("klient - shmget - pools");
        exit(EXIT_FAILURE);
    }
}


void olimpic_join_pool(){
    olimpic_access_pool();
    int* pool =(int*) shmat(OLIMPIC_POOL_SHMID, NULL, 0);
    bool waited_in_queue = true; // If space in pool available skip the queue


    SEM_OPERATE.sem_op = SEM_WAIT;
    semop(OLIMPIC_POOL_SEMID, &SEM_OPERATE, 1);
    POOL_SIZE = *pool;

    if(POOL_SIZE < POOL_OLIMPIC_MAX_SIZE){
        waited_in_queue = false;
    }
    else {
        set_client_info(ACTION__ENTERED, LOCATION__OLIMPIC_QUEUE, REASON__NOT_ENOUGH_SPACE);
        log_pool_data(olimpic_get_data, STATUS_NONE);
    }

    SEM_OPERATE.sem_op = SEM_SIGNAL;
    semop(OLIMPIC_POOL_SEMID, &SEM_OPERATE, 1); 


    // Enter olimpic pool queue
    if(msgrcv(OLIMPIC_POOL_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), MSQ_POOL_SPACE, 0) == FAILURE && errno != EINTR){
        perror("klient - msgrcv");
        exit(EXIT_FAILURE);
    }
    if(waited_in_queue) set_client_info(ACTION__LEFT, LOCATION__OLIMPIC_QUEUE, REASON__SPACE_AVAILABLE), log_client(WHO__CLIENT);



    // Enter olimpic pool
    SEM_OPERATE.sem_op = SEM_WAIT;
    semop(OLIMPIC_POOL_SEMID, &SEM_OPERATE, 1);
    *pool += 1;
    POOL_SIZE = *pool;
    set_client_info(ACTION__ENTERED, LOCATION__OLIMPIC_POOL, REASON__NONE);
    log_pool_data(olimpic_get_data, STATUS_ENTER);

    SEM_OPERATE.sem_op = SEM_SIGNAL;
    semop(OLIMPIC_POOL_SEMID, &SEM_OPERATE, 1);


    sleep(1);


    // Make space for others
    SEM_OPERATE.sem_op = SEM_WAIT;
    semop(OLIMPIC_POOL_SEMID, &SEM_OPERATE, 1);

    *pool -= 1;
    POOL_SIZE = *pool;
    set_client_info(ACTION__LEFT, LOCATION__OLIMPIC_POOL, REASON__END_OF_SWIM_TIME);
    log_pool_data(olimpic_get_data, STATUS_LEAVE);
    shmdt(pool);

    SEM_OPERATE.sem_op = SEM_SIGNAL;
    semop(OLIMPIC_POOL_SEMID, &SEM_OPERATE, 1);
    
    MSQ_BUFFER.mtype=MSQ_POOL_SPACE;
    if(msgsnd(OLIMPIC_POOL_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), 0) == FAILURE){
        perror("klient - msgsnd - olimpic");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}


// -------------------- Leisure pool --------------------
void leisure_access_pool(){
    SEM_OPERATE.sem_flg = 0;
    SEM_OPERATE.sem_num = SEM_POOL_SHM;

    key_t key_msq, key_sem, key_shm;

    key_msq = get_key(KEY_LEISURE_POOL_MSQ);
    key_sem = get_key(KEY_LEISURE_POOL_SEM);
    key_shm = get_key(KEY_LEISURE_POOL_SHM);

    if((LEISURE_POOL_MSQID = msgget(key_msq, 0600)) == FAILURE){
        perror("klient - msgget - pools");
        exit(EXIT_FAILURE);
    }

    if((LEISURE_POOL_SEMID = semget(key_sem, SEM_POOL_NUM, 0600)) == FAILURE){
        perror("klient - semget - pools");
        exit(EXIT_FAILURE);
    }

    if((LEISURE_POOL_SHMID = shmget(key_shm, 2*sizeof(int), 0600)) == FAILURE){
        perror("klient - shmget - pools");
        exit(EXIT_FAILURE);
    }
}


void leisure_join_pool(){
    set_client_info(ACTION__ENTERED, LOCATION__LEISURE_POOL, REASON__NONE), log_client(WHO__CLIENT);
    if(client_has_child()){
        child.tid = new_thread(child_enter_complex, NULL);
        pthread_join(child.tid, NULL);
    }

    while(true);
}


// -------------------- Paddling pool --------------------
void paddling_access_pool(){
    SEM_OPERATE.sem_flg = 0;
    SEM_OPERATE.sem_num = SEM_POOL_SHM;

    key_t key_msq, key_sem, key_shm;

    key_msq = get_key(KEY_PADDLING_POOL_MSQ);
    key_sem = get_key(KEY_PADDLING_POOL_SEM);
    key_shm = get_key(KEY_PADDLING_POOL_SHM);

    if((PADDLING_POOL_MSQID = msgget(key_msq, 0600)) == FAILURE){
        perror("main - msgget - pools");
        exit(EXIT_FAILURE);
    }

    if((PADDLING_POOL_SEMID = semget(key_sem, SEM_POOL_NUM, 0600)) == FAILURE){
        perror("main - semget - pools");
        exit(EXIT_FAILURE);
    }

    if((PADDLING_POOL_SHMID = shmget(key_shm, sizeof(int), 0600)) == FAILURE){
        perror("main - shmget - pools");
        exit(EXIT_FAILURE);
    }
}


void paddling_join_pool(){
    paddling_access_pool();


    // Enter paddling pool queue
    set_client_info(ACTION__ENTERED, LOCATION__PADDLING_QUEUE, REASON__NONE), log_client(WHO__CLIENT);
    child.tid = new_thread(child_keep_eye_on, NULL);
    pthread_join(child.tid, NULL);
    if(msgrcv(PADDLING_POOL_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), MSQ_POOL_SPACE, 0) == FAILURE && errno != EINTR){
        perror("klient - msgrcv - paddling");
        exit(EXIT_FAILURE);
    }
    set_client_info(ACTION__LEFT, LOCATION__PADDLING_QUEUE, REASON__NONE), log_client(WHO__CLIENT);
    child.tid = new_thread(child_keep_eye_on, NULL);
    pthread_join(child.tid, NULL);


    // Enter paddling pool
    set_client_info(ACTION__ENTERED, LOCATION__PADDLING_POOL, REASON__SPACE_AVAILABLE), log_client(WHO__CLIENT);
    child.tid = new_thread(child_keep_eye_on, NULL);
    pthread_join(child.tid, NULL);


    // Make space for others
    set_client_info(ACTION__LEFT, LOCATION__PADDLING_POOL, REASON__END_OF_SWIM_TIME), log_client(WHO__CLIENT);
    child.tid = new_thread(child_keep_eye_on, NULL);
    pthread_join(child.tid, NULL);
    MSQ_BUFFER.mtype=MSQ_POOL_SPACE;
    if(msgsnd(PADDLING_POOL_MSQID, &MSQ_BUFFER, sizeof(MSQ_BUFFER.mvalue), 0) == FAILURE){
        perror("klient - msgsnd - paddling");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
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