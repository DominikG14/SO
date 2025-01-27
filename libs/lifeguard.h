#include "config.h"
#include "logging.h"
#include "global.h"
#include "random.h"


// -------------------- Local --------------------
bool GUARD = true;
int GUARDED_POOL;


// -------------------- Lifeguard functionality --------------------
void close_pool(){

}

void open_pool(){

}


// -------------------- IPCS --------------------
void __close_complex_handler(){
    GUARD = false;
}


void __set_close_complex_handler(){
    struct sigaction handler;
    handler.sa_handler = __close_complex_handler;
    sigemptyset(&handler.sa_mask);
    handler.sa_flags = 0;
    sigaction(SIG_CLOSE_COMPLEX, &handler, 0);
}


void setup(){
    __set_close_complex_handler();
    set_config_vars();
}