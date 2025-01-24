#pragma once

#include "logging.h"
#include "data_client.h"


void log_pool_data(int ID, int WHO, int ACTION, int LOCATION, int REASON, char* (*data)(), int status){
    __log_console(ID, WHO, ACTION, LOCATION, REASON);
    char* return_data = data(status);
    printf("\n");

    log_file(LOGGING_FILEPATH, ID, WHO, ACTION, LOCATION, REASON, return_data);

    free(return_data), return_data = NULL;
}