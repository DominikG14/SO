#pragma once

#include "pool.h"
#include "low.h"
#include "vars.h"
#include "color.h"


char* STATUS_NEW = "new";
char* STATUS_PREV = "prev";
char* STATUS_NONE = "";


double leisure_age_avg(LeisurePool* pool, int additional_age, int additional_size){
    int age_sum = pool->age_sum + additional_age;
    int size_sum = pool->size + additional_size;

    double age_avg =(double) age_sum / size_sum;

    return age_avg;
}


void disp_leisure_data(){
    key_t key = get_key(POOL_LEISURE_KEY_ID);
    int pool_shmid = access_shared_mem(key, POOL_SHARED_MEM_SIZE[LEISURE], 0600);
    LeisurePool* pool =(LeisurePool*) get_shared_mem(pool_shmid);

    // Pool size
    int num;
    if(client_has_child()) num = 2;
    else num = 1;

    printf_clr(cyan, "| ");
    printf("size: %d/%d (new: %d/%d)", pool->size, POOL_SIZE[LEISURE],pool->size + num, POOL_SIZE[LEISURE]);

    // Pool age sum
    int age;
    if(client_has_child()) age = client.age + child.age;
    else age = client.age;

    printf_clr(cyan, " | ");
    printf("age_sum: %d (new: %d)", pool->age_sum, pool->age_sum + age);

    // Pool age avg
    printf_clr(cyan, " | ");
    printf("age_avg: %.2f/%d (new %.2f/%d)", leisure_age_avg(pool, 0, 0), POOL_LEISURE_AGE_AVG, leisure_age_avg(pool, age, num), POOL_LEISURE_AGE_AVG);

    // End
    printf_clr(cyan, " |");

    detach_shared_mem(pool);
}


void disp_olimpic_data(char* status){
    key_t key = get_key(POOL_OLIMPIC_KEY_ID);
    int pool_shmid = access_shared_mem(key, POOL_SHARED_MEM_SIZE[OLIMPIC], 0600);
    OlimpicPool* pool =(OlimpicPool*) get_shared_mem(pool_shmid);

    // Pool size
    if(strcmp(status, STATUS_NONE) == 0){
        printf("size: %d/%d", pool->size, POOL_SIZE[OLIMPIC]);
    }
    else{
        int num;
        if(strcmp(status, STATUS_NEW) == 0) num = 1;
        else num = -1;

        printf_clr(cyan, "| ");
        printf("size: %d/%d (%s: %d/%d)", pool->size, POOL_SIZE[OLIMPIC], status, pool->size + num, POOL_SIZE[OLIMPIC]);
    }
    

    // End
    printf_clr(cyan, " |");

    detach_shared_mem(pool);
}


void disp_paddling_data(){
    key_t key = get_key(POOL_PADDLING_KEY_ID);
    int pool_shmid = access_shared_mem(key, POOL_SHARED_MEM_SIZE[PADDLING], 0600);
    PaddlingPool* pool =(PaddlingPool*) get_shared_mem(pool_shmid);

    // Pool size
    printf_clr(cyan, "| ");
    printf("size: %d/%d (new: %d/%d)", pool->size, POOL_SIZE[PADDLING], pool->size + 2, POOL_SIZE[PADDLING]);

    // End
    printf_clr(cyan, " |");

    detach_shared_mem(pool);
}


void disp_client_data(){
    // Client Age
    printf_clr(blue, "| ");
    printf("age: %d", client.age);

    // Client swim cap
    printf_clr(blue, " | ");
    printf("swim_cap_on: ");
    if(client.swim_cap_on) printf_clr(green, "yes");
    else printf_clr(red, "no");

    // Client child
    printf_clr(blue, " | ");
    printf("with_child: ");
    if(client_has_child()) printf_clr(green, "yes");
    else printf_clr(red, "no");

    // End
    printf_clr(blue, " |");
}


void disp_child_data(){
    // Child Parent

    // Child Age
    printf_clr(green, "| ");
    printf("age: %d", child.age);

    // Child swim cap
    printf_clr(green, " | ");
    printf("swim_cap_on: ");
    if(child.swim_cap_on) printf_clr(green, "yes");
    else printf_clr(red, "no");

    // Child diaper
    printf_clr(green, " | ");
    printf("diaper_on: ");
    if(child.diaper_on) printf_clr(green, "yes");
    else printf_clr(red, "no");

    // End
    printf_clr(green, " |");
}
