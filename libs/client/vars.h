#pragma once

#include "struct.h"


// Config
int CLIENT_HAS_CHILD_PERC;
int CLIENT_SWIM_CAP_PREC;

int CLIENT_MIN_SWIM_TIME;
int CLIENT_MAX_SWIM_TIME;

int CLIENT_MIN_AGE;
int CLIENT_MAX_AGE;

int CHILD_MIN_AGE;
int CHILD_MAX_AGE;

int CHILD_BABY_AGE;

int POOL_LEISURE_AGE_AVG;
int POOL_SIZE[3];


// Local
Client client;
Child  child;
int CURRENT_POOL;