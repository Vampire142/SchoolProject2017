#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "RS232.h"

struct sockaddr_rc rem_addr = {0}, loc_addr = {0};
pthread_mutex_t mutex;
sem_t sem;
char buffer[64] = {0};
int sockfd, client;
char rc_addr[8] = {0};

socklen_t length_rem_addr = sizeof(rem_addr);
socklen_t length_loc_addr = sizeof(loc_addr);

void *sendOutput();

void main()
{
    printf("Démarrage Server Bluetooth\n");
    pthread_t Reception, Transmission;

    sem_init(&sem, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&Reception, NULL, readInput, NULL);

    sockfd = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = 10;

    bind(sockfd, (struct sockaddr *)&loc_addr, length_loc_addr);

    listen(sockfd, 1);

    printf("Attente de client\n");
    while (1)
    {
        client = accept(sockfd, (struct sockaddr *)&rem_addr, &length_rem_addr);

        ba2str(&rem_addr.rc_bdaddr, rc_addr);
        printf("Connexion recu de : %s\n", rc_addr);

        pthread_create(&Transmission, NULL, sendOutput, NULL);
    }

    close(client);
    close(sockfd);
}

void *sendOutput()
{
    printf("Thread envoie cree !\n");
    while (client > 0)
    {
        //pthread_mutex_lock(&mutex);
        sem_wait(&sem);

        if (send(sockfd, buffer, (size_t)strlen(buffer) + 1, 0) < 0)
            printf("Le client c'est déconnecter !\n");

        //pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}
