#pragma once

#include "global.h"
#include "colors.h"
#include "logging.h"


int CLIENT_LOCATION;
int CLIENT_ACTION;
int CLIENT_REASON;


void olimpic_join_pool(){
    CLIENT_LOCATION = LOCATION__OLIMPIC_POOL;
    printf_clr(blue, "%d: klient wszedl do basenu olimpijskiego\n", getpid());

    while(true);
}


void leisure_join_pool(){
    CLIENT_LOCATION = LOCATION__LEISURE_POOL;
    printf_clr(blue, "%d: klient wszedl do basenu rekreacyjnego\n", getpid());

    while(true);
}


void paddling_join_pool(){
    CLIENT_LOCATION = LOCATION__PADDLING_POOL;
    printf_clr(blue, "%d: klient wszedl do brodziku\n", getpid());

    while(true);
}