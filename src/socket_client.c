#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define LOCAL_PORT 0
#define MAX_CHAR 80
#define NBMAX_THREADS 2

#define CHECK_ERROR(var, val, message){if(var==val){perror( message);return -1;}}
#define CHECK_T(status, msg)                        \
  if (0 != (status))   {                            \
    fprintf(stderr, "pthread erreur : %s\n", msg);  \
    exit (-1);                                 \
  }

typedef void * (*pf_t)(void *);
int sd; // Socket de dialogue
char bufflect [MAX_CHAR+1];

void threadSend(char * trame);
void threadRead(void);
void affiche(char buff[], int len);

int main(int argc, char **argv)
{
    printf("START PROGRAM");

    if(argc<5){
        CHECK_ERROR(argc, 5, "Il manque des params");
        return -1;
    }

    char * IP_CLIENT = argv[1];
    char * IP_SERVEUR = argv[2];
    int PORT_SERVEUR = atoi(argv[3]);
    char * XWAY_ADDRESS = "1E";
    pthread_t tid[NBMAX_THREADS];

    printf("IP_CLIENT: %s \n", IP_CLIENT);
    printf("IP_SERVEUR: %s \n", IP_SERVEUR);
    printf("PORT_SERVEUR: %d \n", PORT_SERVEUR);
    printf("XWAY_ADDRESS: %s \n", XWAY_ADDRESS);

    struct sockaddr_in adrlect;
    struct sockaddr_in adrecriv;
    int tailleadr;

    char trame [27];

    // deuxième trame

    trame[0] = 0x00;
    trame[1] = 0x00;
    trame[2] = 0x00;
    trame[3] = 0x01;
    trame[4] = 0x00;
    trame[5] = 0x14;
    trame[6] = 0x00;
    trame[7] = 0xF0;
    trame[8] = 0x0C;
    trame[9] = 0x10;
    trame[10] = 0x14;
    trame[11] = 0x10;
    trame[12] = 0x37;
    trame[13] = 0x06;
    trame[14] = 0x68;
    trame[15] = 0x07;
    trame[16] = 0x32;
    trame[17] = 0x00;
    trame[18] = 0x03;
    trame[19] = 0x00;
    trame[20] = 0x01;
    trame[21] = 0x00;
    trame[22] = 0x02;
    trame[23] = 0x00;
    trame[24] = 0x03;
    trame[25] = 0x00;


    //creation d'un pts de communication

    sd = socket(AF_INET, SOCK_STREAM, 0);

    CHECK_ERROR(sd, -1, "Erreur de creation de la socket !!! \n");

    // Definition de l'adresse de l'ecrivain

    adrlect.sin_family=AF_INET;
    adrlect.sin_port=htons(PORT_SERVEUR); // def du numero de port de la socket
    adrlect.sin_addr.s_addr=inet_addr(IP_SERVEUR); // def de l'addresse  de la socket

    // Associer l'adresse à la socket
    CHECK_ERROR(connect(sd,(const struct sockaddr *) &adrlect, sizeof(adrlect)), -1, "FAIL TO CONNECT!!! \n");



    // Definition de l'adresse du lecteur

    adrlect.sin_family=AF_INET;
    adrlect.sin_port=htons(PORT_SERVEUR); // def du numero de port de la socket
    adrlect.sin_addr.s_addr=inet_addr(IP_SERVEUR); // def de l'addresse  de la socket

    // CHECK_ERROR(send(sd, (void *) trame, 26, 0), -1, "Erreur envoi data !!! \n");


    // while(1)
    // {
    //     printf("hile");
    //     int nb = -1;
    //     nb = recv(sd, (void *) bufflect, MAX_CHAR+1, 0);
    //     CHECK_ERROR(nb, -1, "Erreur reception data !!! \n");
    //     printf("Just Received %s \n", bufflect);
    //     printf("nb = %d\n", nb);
    // }

    CHECK_T(pthread_create (&tid[0], NULL, (pf_t)threadSend,
                                (char *)trame), "pthread_create()");
    CHECK_T(pthread_create (&tid[1], NULL, (pf_t)threadRead,
                                NULL), "pthread_create()");

    getchar();
    close(sd);
    return 1;
}

void threadSend(char * trame)
{
    printf("START SEND \n");
    CHECK_ERROR(send(sd, (void *) trame, 26, 0), -1, "Erreur envoi data !!! \n");
	pthread_exit((void *) 1);
}

void threadRead(void)
{
    printf("START READ \n");
    char trame [15];
    while(1)
    {
        int nb = -1;
        nb = recv(sd, (void *) bufflect, MAX_CHAR+1, 0);
        CHECK_ERROR(nb, -1, "Erreur reception data !!! \n");
        printf("Just Received\n");
        affiche(bufflect, nb);
        printf("nb = %d\n", nb);
        if (nb > 16)
        {
            int seqAlea = bufflect[13];
            trame[0] = 0x00;
            trame[1] = 0x00;
            trame[2] = 0x00;
            trame[3] = 0x01;
            trame[4] = 0x00;
            trame[5] = 0x09;
            trame[6] = 0x00;
            trame[7] = 0xF1;
            trame[8] = 0x0C;
            trame[9] = 0x10;
            trame[10] = 0x14;
            trame[11] = 0x10;
            trame[12] = 0x19;
            trame[13] = seqAlea;
            trame[14] = 0xFE;
            CHECK_ERROR(send(sd, (void *) trame, 15, 0), -1, "Erreur envoi data !!! \n");
        }
    }
}

void affiche(char buff[], int len){
    int i = 0;
    while (i < len){
        printf("%x", buff[i]);
        i++;
        if (i < len)
        {
            printf("%x", buff[i]);
            i++;
        }
        printf(" ");
    }
    printf("\n");
}