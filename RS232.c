#include "RS232.h"
#include <time.h>
#include <stdlib.h>

int *readInput()
{
    while (1) //Thread 1
    {
        
        
        srand(time(NULL));
        for (int i = 0; i < 8; ++i)
        {
            buffer[i] = '0' + rand() % 72;
        }
        // Fin générateur de string aléatoire

        printf("readInput() : buffer = %s\n", buffer);

        sem_post(&sem);
        sleep(5);
    }
    pthread_exit(NULL);
    return 0;
}
