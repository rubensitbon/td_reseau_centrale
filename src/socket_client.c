#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define LOCAL_PORT 0
#define MAX_CHAR 80

#define CHECK_ERROR(var, val, message){if(var==val){perror( message);return -1;}}


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

    printf("IP_CLIENT: %s \n", IP_CLIENT);
    printf("IP_SERVEUR: %s \n", IP_SERVEUR);
    printf("PORT_SERVEUR: %d \n", PORT_SERVEUR);
    printf("XWAY_ADDRESS: %s \n", XWAY_ADDRESS);



    int sd; // Socket de dialogue
    struct sockaddr_in adrlect;
    struct sockaddr_in adrecriv;
    char bufflect [MAX_CHAR+1];
    char buffecrit [MAX_CHAR+1];
    int tailleadr;

    char trame [15];

    trame[0] = 0x00;
    trame[1] = 0x00;
    trame[2] = 0x00;
    trame[3] = 0x01;
    trame[4] = 0x00;
    trame[5] = 0x08;
    trame[6] = 0x00;
    trame[7] = 0xF0;
    trame[8] = 0x1E;
    trame[9] = 0x10;
    trame[10] = 0x14;
    trame[11] = 0x10;
    trame[12] = 0x24;
    trame[13] = 0x06;

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

    CHECK_ERROR(send(sd, (void *) trame, 14, 0), -1, "Erreur envoie data !!! \n");


    // printf("Bien joué roger !");
     getchar();

    /*while(1)
    {
        printf("Client> ");
        gets(buffecrit);

        CHECK_ERROR(send(sd, (void *) buffecrit, strlen(buffecrit)+1, 0), -1, "Erreur envoie data !!! \n");

        CHECK_ERROR(recv(sd, (void *) bufflect, MAX_CHAR+1, 0), -1, "Erreur reception data !!! \n");
        printf("Lecteur> %s \n", bufflect);

    }*/

    close(sd);


    return 1;
}
