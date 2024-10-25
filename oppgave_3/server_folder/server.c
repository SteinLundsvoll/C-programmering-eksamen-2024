#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "server.h"


int main(int argc, char *argv[]){
	int sockFd, new_socket, c;
	int bytesRecieved;
	char buffer[BUFFERSIZE];
	char fileNameInput[BUFFERSIZE];

	// Forsikrer riktig antall argumenter ved oppstart
	if(argc < 2){ // Tillater fler argumenter enn argc kun for å kunne kjøre valgrind
		printf("Not enough arguments.\n");
		printf("File name & Portnumber needed\n");
		exit(EXIT_FAILURE);
	}
	
	// Socket(int domain, int type, int protocol)
	sockFd = socket(AF_INET, SOCK_STREAM, 0); // Creates socket
	if(sockFd < 0){
		printf("Error opening socket\n");
		return ERROR;
	}
	
	struct sockaddr_in saAddr = {0}; 
	int port = atoi(argv[1]); // Kommandolinjen bestemmer portnummer
	saAddr.sin_family = AF_INET;
	saAddr.sin_port = htons(port);
	saAddr.sin_addr.s_addr = INADDR_ANY;
	
	// Binds function is used to assign an adress to the created socked
	if(bind(sockFd, (struct sockaddr *) &saAddr, sizeof(saAddr)) < 0){
		printf("Error on binding\n");
		close(sockFd);
		exit(EXIT_FAILURE);
	}else{
		printf("Binding succeeded!\n");
	}

	// Venter på tilkobling
	listen(sockFd, 5);
	
	// Aksepterer tilkobling
	printf("Waiting for incoming connections...\n");
	c = sizeof(struct sockaddr_in);
	new_socket = accept(sockFd, (struct sockaddr *)&saAddr, (socklen_t*)&c);
	if(new_socket < 0){
		perror("Accept failed\n");
		close(sockFd);
		exit(EXIT_FAILURE);
	}else{
		printf("Connection established\n");
	}
	
	// Tar imot brukernavn og passord og kjører autentiseringsfunksjon
	authenticateClient(new_socket);
	
	bytesRecieved = recv(new_socket, buffer, sizeof(buffer), 0);
	if(bytesRecieved < 0){
		printf("Couldn't recieve GET or PUT commands\n");
		close(new_socket);
		exit(EXIT_FAILURE);
	}
	
	buffer[bytesRecieved] = '\0'; // Null terminering
	bytesRecieved = 0;
	
	if(strcasecmp (buffer, "GET") == 0){
		printf("GET request recieved\n");
		
		bytesRecieved = recv(new_socket, fileNameInput, sizeof(fileNameInput), 0);
		if(bytesRecieved < 0){
			printf("Couldn't recieve filename form client\n");
			close(new_socket);
			exit(EXIT_FAILURE);
		}
		
		sendFileToClient(new_socket, fileNameInput);
		
	}else if(strcasecmp (buffer, "PUT") == 0){
		// Tar imot filen tilsendt fra client
		printf("PUT request recieved\n");
		recieveClientFile(new_socket);
	}
	
	
	close(sockFd);
	return 0;
}

int authenticateClient(int new_socket){
	char userNameBuffer[256];
	char passwordBuffer[256];
	int bytesRecieved = 0;
	int rc = ERROR;
	
	// Mottar brukernavn fra klient
	bytesRecieved = recv(new_socket, userNameBuffer, sizeof(userNameBuffer), 0);
	if(bytesRecieved < 0){
		printf("Failed to recieve client username...\n");
		close(new_socket);
		exit(EXIT_FAILURE);
	}
	
	userNameBuffer[bytesRecieved] = '\0'; // Null terminering
	bytesRecieved = 0; // For å klarere input før den brukes på ny
	
	// Mottar passord fra klient
	bytesRecieved = recv(new_socket, passwordBuffer, sizeof(passwordBuffer), 0);
	if(bytesRecieved < 0){
		printf("Failed to recieve client password...\n");
		close(new_socket);
		exit(EXIT_FAILURE);
	}
	
	passwordBuffer[bytesRecieved] = '\0'; // Null terminering
	
	if(ftp_check_user(userNameBuffer, passwordBuffer) != 1){
		printf("Incorrect username or password...\n");
		//printf("Username: %s\n", userNameBuffer);
		//printf("Password: %s\n", passwordBuffer);
		close(new_socket);
		exit(EXIT_FAILURE);
	}else{
		printf("%s authenticated.\n", userNameBuffer);
		rc = OK;
	}
	
	char response[] = "You are logged in";
    if(send(new_socket, response, strlen(response), 0) < 0){
    	printf("Failed to respond\n");
    	return ERROR;
    }
	
	return rc;
}
