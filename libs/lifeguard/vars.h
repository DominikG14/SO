#pragma once

#include <stdbool.h>


// Config
int LIFEGUARD_CLOSE_POOL_PERC;
int LIFEGUARD_CLOSE_POOL_MIN_TIME;
int LIFEGUARD_CLOSE_POOL_MAX_TIME;

// Local
int GUARDED_POOL;
int POOL_SHMID;
int POOL_SEMID;
int POOL_SIZE[0];