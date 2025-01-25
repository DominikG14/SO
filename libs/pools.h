#pragma once

#include "global.h"
#include "colors.h"


int CLIENT_LOCATION;


void olimpic_join_pool(){
    CLIENT_LOCATION = LOCATION_OLIMPIC_POOL;
    printf_clr(blue, "%d: klient wszedl do basenu olimpijskiego\n", getpid());
}

void leisure_join_pool(){
    CLIENT_LOCATION = LOCATION_LEISURE_POOL;
    printf_clr(blue, "%d: klient wszedl do basenu rekreacyjnego\n", getpid());
}

void paddling_join_pool(){
    CLIENT_LOCATION = LOCATION_PADDLING_POOL;
    printf_clr(blue, "%d: klient wszedl do brodziku\n", getpid());
}