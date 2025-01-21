#pragma once


enum KEYS {
    OLIMPIC_POOL__KEY_ID,
    LEISURE_POOL__KEY_ID,
    PADDLING_POOL__KEY_ID,
    CASHIER__KEY_ID,
};


char* CASHIER__TMP_FILE_MSQID = "./tmp/cash_msqid";
char* CASHIER__TMP_FILE_SEMID = "./tmp/cash_semid";


enum SEM__CASHIER_SEMS {
    SEM__CASHIER_PAYMENT,
    SEM__CASHIER_STATUS,
};