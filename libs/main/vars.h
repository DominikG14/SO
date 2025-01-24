#pragma once

#include <stdbool.h>


// Config
int TIME_PER_SEC;
int TIME_START;
int TIME_END;
int COMPLEX_OPEN_TIME;
int COMPLEX_CLOSE_TIME;
int CLIENT_SPAWN_PERC;

// Local
int TIME_CURR;
bool COMPLEX_IS_OPEN;
int LOGGER_MSQID;

int CLIENT_PIDS[1500];
int CLIENTS_NUM = 0;
int CASHIER_PID;
int LIFEGUARD_PIDS[3];