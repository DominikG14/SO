#pragma once

#include <stdbool.h>
#include "vars.h"


bool client_is_underage(){
    return client.age < 18; 
}


bool client_has_child(){
    return child.tid != -1;
}


bool client_has_baby(){
    return client_has_child() && child.age <= CHILD_BABY_AGE;
}