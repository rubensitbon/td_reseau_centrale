#première règle : règle par défaut 

all: socket_client socket_serveur


socket_client: build/socket_client
socket_serveur: build/socket_serveur

INC=-I./include
CC=gcc $(INC)

build/socket_client: src/socket_client.c
	$(CC) src/socket_client.c -o build/socket_client


build/socket_serveur: src/socket_serveur.c
	$(CC) src/socket_serveur.c -o build/socket_serveur

clean: 
	@echo "Nettoyage"
	@rm -rf build/*
