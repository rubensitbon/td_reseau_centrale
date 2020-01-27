#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define IP_ADDR_LOCAL "0.0.0.0"
#define LOCAL_PORT 2000
#define MAXCHAR 80


#define CHECK_T(status, msg)                        \
  if (0 != (status))   {                            \
    fprintf(stderr, "pthread erreur : %s\n", msg);  \
    exit (EXIT_FAILURE);                            \
  }

#define CHECK_ERROR(var, val, message){if(var==val){perror( message);return -1;}}


int main(int argc, char **argv) {
    int se, sd; // Socket d'écoute et de dialogue
    struct sockaddr_in adrlect; // Adresse du lecteur
    struct sockaddr_in adrecriv; // Adresse de l'écrivain
    char bufflect[MAXCHAR + 1];
    char buffecri[MAXCHAR + 1];
    int tailleadr;

    // Création d'un pont de communication d'écoute
    se = socket(AF_INET, SOCK_STREAM, 0); // Création d'une socket en TCP
    CHECK_ERROR(se, -1, "Erreur creation de la socket\n");

    // Definition de l'addresse du lecteur
    adrlect.sin_family = AF_INET;
    adrlect.sin_port = htons(LOCAL_PORT); // Definition numero port socket
    adrlect.sin_addr.s_addr = inet_addr(IP_ADDR_LOCAL);

    // Definition de l'addresse de l'écrivain
    adrecriv.sin_family = AF_INET;
//    adrecriv.sin_port = htons(LOCAL_PORT); // Definition numero port socket
//    adrecriv.sin_addr.s_addr = inet_addr(IP_ADDR_LOCAL);

    // Associer l'adresse à la socket
    CHECK_ERROR(bind(se, (const struct sockaddr *) &adrlect, sizeof(adrlect)), -1, "Erreur de bind\n")


    // Lire message envoyé par l'écrivain
    while (1) {

        listen(se, 10);

        sd = accept(se, NULL, NULL);
        CHECK_ERROR(sd, -1, "Erreur de bind\n")
        if (fork()) {
            close(sd);
        } else {
            close(se);
            do {
                tailleadr = sizeof(adrecriv);
                //Reception
                CHECK_ERROR(recv(sd, (void *) bufflect, MAXCHAR + 1, 0), -1, "Erreur réception data\n");
                printf("ECRIVAIN> '%s'\n", bufflect);

                printf("LECTEUR>");
                gets(buffecri);
                // Emission
                CHECK_ERROR(send(sd, (void *) buffecri, strlen(buffecri) + 1, 0), -1, "Erreur emission data\n");
            }
            while (1);
            exit(1);
        }
    }

    close(sd); // Fermeture de la socket

    return 1;

}
