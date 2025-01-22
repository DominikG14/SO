#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


enum SIG_ABBR {
    SIG_CLOSE_POOL = SIGUSR1, // Main -> Everyone
    SIG_LEAVE_CASH_CHILD = SIGUSR1, // Client -> Child
};


void send_signal(pid_t pid, int signum){
    kill(pid, signum);
}


void handle_signal(int signum, void (*handler)()){
    signal(signum, handler);
}