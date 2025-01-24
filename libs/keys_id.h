#pragma once

char* LOGGING_FILEPATH = "./logs";

// Keys to access IPCS
enum KEYS {
    POOL_OLIMPIC_KEY_ID,
    POOL_LEISURE_KEY_ID,
    POOL_PADDLING_KEY_ID,
    CASH_KEY_ID,
    LOGGING_KEY_ID,
};


enum SEM_CASH {
    SEM_CASH_PAYMENT, // Waiting to queue
    SEM_CASH_STATUS,  // Queue status, 1 = Closed;
    SEM_CASH_NUM,
};


enum SEM_POOL {
    SEM_POOL_ENTER,
    SEM_POOL_NUM,
};


enum GENERAL_STATE {
    SUCCESS = 0,
    FAILURE = -1
};