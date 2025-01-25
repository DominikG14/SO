#include "main.h"


int pid;


int main(){
    setup();

    // Kasjer otwiera kase
    switch(PID_KASJER = fork()){
        case FAILURE:
            perror("main - fork");
            exit(EXIT_FAILURE);

        case SUCCESS:
            execl(PS_KASJER_PATH, PS_KASJER_NAME, NULL);
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
                execl(PS_KLIENT_PATH, PS_KLIENT_NAME, NULL);
                perror("main - execl");
                exit(EXIT_FAILURE);
            
            default:
                PID_KLIENCI[ILOSC_KLIENTOW] = pid;
                ILOSC_KLIENTOW++;
        }
    }


    // Zamknij kase
    kill(PID_KASJER, SIG_ZAMKNIJ_BASENY);
    waitpid(PID_KASJER, NULL, 0);


    // Poczekaj az wszyscy opuszcza basen
    for(int i = 0; i < ILOSC_KLIENTOW; i++){
        kill(PID_KLIENCI[i], SIG_ZAMKNIJ_BASENY);
        waitpid(PID_KLIENCI[i], NULL, 0);
    }


    // Wyczysc pamiec dzielona
    cleanup();
    return 0;
}