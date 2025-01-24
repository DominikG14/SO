#pragma once

#include "pool.h"
#include "low.h"
#include "vars.h"
#include "color.h"


enum {
    STATUS_ENTER,
    STATUS_LEAVE,
    STATUS_NONE,
};


int POOL_SHMID;
int POOL_SEMID;


double leisure_age_avg(int additional_age, int additional_size){
    LeisurePool* pool =(LeisurePool*) get_shared_mem(POOL_SHMID);
    int age_sum = pool->age_sum + additional_age;
    int size_sum = pool->size + additional_size;

    double age_avg;
    if(size_sum != 0){
        age_avg =(double) age_sum / size_sum;
    }
    else age_avg = 0;


    detach_shared_mem(pool);
    return age_avg;
}


char* leisure_data(int status){
    LeisurePool* pool =(LeisurePool*) get_shared_mem(POOL_SHMID);

    // Pool size
    int num;
    if(client_has_child()) num = 2;
    else num = 1;

    int age;
    if(client_has_child()) age = client.age + child.age;
    else age = client.age;

    printf_clr(cyan, "| ");
    switch(status){
        case STATUS_ENTER:
            printf("size: %d/%d (prev: %d/%d)", pool->size, POOL_SIZE[LEISURE], pool->size - num, POOL_SIZE[LEISURE]);
            printf_clr(cyan, " | ");
            printf("age_sum: %d (prev: %d)", pool->age_sum, pool->age_sum - age);
            printf_clr(cyan, " | ");
            printf("age_avg: %.2f/%d (prev: %.2f/%d)", leisure_age_avg(0, 0), POOL_LEISURE_AGE_AVG, leisure_age_avg(-age, -num), POOL_LEISURE_AGE_AVG);
            break;

        case STATUS_LEAVE:
            printf("size: %d/%d (prev: %d/%d)", pool->size, POOL_SIZE[LEISURE], pool->size + num, POOL_SIZE[LEISURE]);
            printf_clr(cyan, " | ");
            printf("age_sum: %d (prev: %d)", pool->age_sum, pool->age_sum + age);
            printf_clr(cyan, " | ");
            printf("age_avg: %.2f/%d (prev: %.2f/%d)", leisure_age_avg(0, 0), POOL_LEISURE_AGE_AVG, leisure_age_avg(age, num), POOL_LEISURE_AGE_AVG);
            break;

        case STATUS_NONE:
            printf("size: %d/%d", pool->size, POOL_SIZE[LEISURE]);
            printf_clr(cyan, " | ");
            printf("age_sum: %d", pool->age_sum);
            printf_clr(cyan, " | ");
            printf("age_avg: %.2f/%d (new: %.2f/%d)", leisure_age_avg(0, 0), POOL_LEISURE_AGE_AVG, leisure_age_avg(age, num), POOL_LEISURE_AGE_AVG);
    }
    printf_clr(cyan, " |");

    // log to file
    char* data =(char*) malloc(4096);
    switch(status){
        case STATUS_ENTER:
            sprintf(data, "| size: %d/%d (prev: %d/%d) | age_sum: %d (prev: %d) | age_avg: %.2f/%d (prev: %.2f/%d) |",
                pool->size, POOL_SIZE[LEISURE], pool->size - num, POOL_SIZE[LEISURE],
                pool->age_sum, pool->age_sum - age,
                leisure_age_avg(0, 0), POOL_LEISURE_AGE_AVG, leisure_age_avg(-age, -num), POOL_LEISURE_AGE_AVG
            );
            break;

        case STATUS_LEAVE:
            sprintf(data, "| size: %d/%d (prev: %d/%d) | age_sum: %d (prev: %d) | age_avg: %.2f/%d (prev: %.2f/%d) |",
                pool->size, POOL_SIZE[LEISURE], pool->size + num, POOL_SIZE[LEISURE],
                pool->age_sum, pool->age_sum + age,
                leisure_age_avg(0, 0), POOL_LEISURE_AGE_AVG, leisure_age_avg(age, num), POOL_LEISURE_AGE_AVG
            );
            break;

        case STATUS_NONE:
            sprintf(data, "| size: %d/%d | age_sum: %d | age_avg: %.2f/%d (new: %.2f/%d) |",
                pool->size, POOL_SIZE[LEISURE],
                pool->age_sum,
                leisure_age_avg(0, 0), POOL_LEISURE_AGE_AVG, leisure_age_avg(age, num), POOL_LEISURE_AGE_AVG
            );
    }

    detach_shared_mem(pool);
    return data;
}


char* olimpic_data(int status){
    OlimpicPool* pool =(OlimpicPool*) get_shared_mem(POOL_SHMID);

    // Client age
    printf_clr(cyan, "| ");
    printf("client_age: %d", client.age);

    // Pool size
    printf_clr(cyan, " | ");
    switch(status){
        case STATUS_ENTER:
            printf("size: %d/%d (prev: %d/%d)", pool->size, POOL_SIZE[OLIMPIC], pool->size - 1, POOL_SIZE[OLIMPIC]);
            break;

        case STATUS_LEAVE:
            printf("size: %d/%d (prev: %d/%d)", pool->size, POOL_SIZE[OLIMPIC], pool->size + 1, POOL_SIZE[OLIMPIC]);
            break;

        case STATUS_NONE:
            printf("size: %d/%d", pool->size, POOL_SIZE[OLIMPIC]);
    }
    printf_clr(cyan, " |");


    // log to file
    char* data =(char*) malloc(4096);
    switch(status){
        case STATUS_ENTER:
            sprintf(data, "| client_age: %d | size: %d/%d (prev: %d/%d) |", client.age, pool->size, POOL_SIZE[OLIMPIC], pool->size - 1, POOL_SIZE[OLIMPIC]);
            break;

        case STATUS_LEAVE:
            sprintf(data, "| client_age: %d | size: %d/%d (prev: %d/%d) |", client.age, pool->size, POOL_SIZE[OLIMPIC], pool->size + 1, POOL_SIZE[OLIMPIC]);
            break;

        case STATUS_NONE:
            sprintf(data, "| client_age: %d | size: %d/%d |", client.age, pool->size, POOL_SIZE[OLIMPIC]);
    }

    detach_shared_mem(pool);

    return data;
}


char* paddling_data(int status){
    PaddlingPool* pool =(PaddlingPool*) get_shared_mem(POOL_SHMID);

    // Child age
    printf_clr(cyan, "| ");
    printf("child_age: %d", child.age);
    printf_clr(cyan, " | ");

    // size
    switch(status){
        case STATUS_ENTER:
            printf("size: %d/%d (prev: %d/%d)", pool->size, POOL_SIZE[PADDLING], pool->size - 2, POOL_SIZE[PADDLING]);
            break;

        case STATUS_LEAVE:
            printf("size: %d/%d (prev: %d/%d)", pool->size, POOL_SIZE[PADDLING], pool->size + 2, POOL_SIZE[PADDLING]);
            break;

        case STATUS_NONE:
            printf("size: %d/%d", pool->size, POOL_SIZE[PADDLING]);
    }
    printf_clr(cyan, " |");


    // log to file
    char* data =(char*) malloc(4096);
    switch(status){
        case STATUS_ENTER:
            sprintf(data, "| child_age: %d | size: %d/%d (prev: %d/%d) |", child.age, pool->size, POOL_SIZE[PADDLING], pool->size - 2, POOL_SIZE[PADDLING]);
            break;

        case STATUS_LEAVE:
            sprintf(data, "| child_age: %d | size: %d/%d (prev: %d/%d) |", child.age, pool->size, POOL_SIZE[PADDLING], pool->size + 2, POOL_SIZE[PADDLING]);
            break;

        case STATUS_NONE:
            sprintf(data, "| child_age: %d | size: %d/%d |", child.age, pool->size, POOL_SIZE[PADDLING]);
    }

    detach_shared_mem(pool);
    return data;
}


char* client_data(){
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
    char* data =(char*) malloc(4096);
    sprintf(data, "| age: %d | swim_cap_on: %s | with_child: %s |", client.age, swim_cap, has_child);

    return data;
}


char* child_data(){
    // Child Parent

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
    char* data =(char*) malloc(4096);
    sprintf(data, "| age: %d | swim_cap_on: %s | diaper_on: %s |", child.age, swim_cap, diaper);

    return data;
}
