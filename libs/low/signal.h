#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


enum SIG_ABBR {
    SIG_CLOSE_POOL = SIGUSR1, // Main -> Everyone
    SIG_LIFEGUARD_CLOSE_POOL = SIGUSR2,
};


void send_signal(pid_t pid, int signum){
    kill(pid, signum);
}


void handle_signal(int signum, void (*handler)()){
    signal(signum, handler);
}