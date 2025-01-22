#pragma once


enum KEYS {
    OLIMPIC_POOL__KEY_ID,
    LEISURE_POOL__KEY_ID,
    PADDLING_POOL__KEY_ID,
    CASH_KEY_ID,
};


char* CASHIER__TMP_FILE_MSQID = "./tmp/cash_msqid";
char* CASHIER__TMP_FILE_SEMID = "./tmp/cash_semid";


enum SEM__CASHIER_SEMS {
    SEM_CASH_PAYMENT,
    SEM_CASH_STATUS,
    SEM_CASH_NUM,
};