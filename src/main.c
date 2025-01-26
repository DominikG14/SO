#include "main.h"
#include "logging.h"

int pid;


int main(){
    setup();
    log_console(WHO__POOL_COMPLEX, ACTION__OPENED, LOCATION__POOL_COMPLEX, REASON__NONE);

    open_pools();

    // Kasjer otwiera kase
    switch(PID_CASHIER = fork()){
        case FAILURE:
            perror("main - fork");
            exit(EXIT_FAILURE);

        case SUCCESS:
            execl(PS_CASHIER_PATH, PS_CASHIER_NAME, NULL);
            perror("main - execl");
            exit(EXIT_FAILURE);
    }


    // Wpusc klientow do kolejki
    for(int i = 0; i < 10; i++){
        switch(pid = fork()){
            case FAILURE:
                perror("main - fork");
                exit(EXIT_FAILURE);

            case SUCCESS:
                execl(PS_CLIENT_PATH, PS_CLIENT_NAME, NULL);
                perror("main - execl");
                exit(EXIT_FAILURE);
            
            default:
                PID_CLIENTS[CLIENTS_NUM] = pid;
                CLIENTS_NUM++;
        }
    }

    sleep(3);
    log_console(WHO__POOL_COMPLEX, ACTION__CLOSED, LOCATION__POOL_COMPLEX, REASON__NONE);

    // Zamknij kase
    kill(PID_CASHIER, SIG_CLOSE_COMPLEX);
    waitpid(PID_CASHIER, NULL, 0);


    // Poczekaj az wszyscy opuszcza basen
    for(int i = 0; i < CLIENTS_NUM; i++){
        kill(PID_CLIENTS[i], SIG_CLOSE_COMPLEX);
        waitpid(PID_CLIENTS[i], NULL, 0);
    }


    // Wyczysc pamiec dzielona
    clean_up();
    exit(EXIT_SUCCESS);
}