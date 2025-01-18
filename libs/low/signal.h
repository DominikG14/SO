#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


void send_signal(pid_t pid, int signum){
    kill(pid, signum);
}


void handle_signal(int signum, void (*handler)()){
    signal(signum, handler);
}