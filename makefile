#première règle : règle par défaut 

all: @echo choisir une règle parmi  types,karaoks

sockets: build/socket_client_dyn build/socket_client_static build/socket_serveur_dyn build/socket_serveur_static

socket_client: build/socket_client_dyn build/socket_client_static
socket_serveur: build/socket_serveur_dyn build/socket_serveur_static

CC=gcc
DEP=src/socket_client.c src/socket_serveur.c

build/socket_client_dyn: $(DEP)
	$(CC) src/socket_client.c -o build/socket_client_dyn

build/socket_client_static: $(DEP)
	$(CC) -static src/socket_client.c -o build/socket_client_static

build/socket_serveur_dyn: $(DEP)
	$(CC) src/socket_serveur.c -o build/socket_serveur_dyn

build/socket_serveur_static: $(DEP)
	$(CC) -static src/socket_serveur.c -o build/socket_serveur_static

clean: 
	@echo "Nettoyage"
	@rm -rf build/*
