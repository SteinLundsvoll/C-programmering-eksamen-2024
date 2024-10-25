#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "client.h"

int main(int argc, char *argv[]){
	int sockFd, e;
	char inputBuffer[BUFFERSIZE] = {0};

	// Forsikrer riktig antall argumenter ved oppstart
	if(argc < 3){ // Tillater fler argumenter enn argc kun for å kunne kjøre valgrind
		printf("Not enough arguments\n");
		printf("Filename, Portnumber & IP Adress needed.\n");
		exit(EXIT_FAILURE);
	}
	
	// Creates socket
	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockFd < 0){
		printf("Error on opening socket\n");
	}
	else{
		printf("Socket created\n");
	}
	struct sockaddr_in saAddr = {0}; 
	int port = atoi(argv[1]); // Kommandolinjen bestemmer portnummer
	saAddr.sin_family = AF_INET;
	saAddr.sin_port = htons(port);
	saAddr.sin_addr.s_addr = inet_addr(argv[2]); //127.0.0.1 | 0x7F000001
	e = connect(sockFd, (struct sockaddr *)&saAddr, sizeof(saAddr));
	if(e < 0){
		printf("Error connecting\n");
		close(sockFd);
		exit(EXIT_FAILURE);
	}else{
		printf("Connected to server\n");
	}
	
	// Funksjon for å sende brukernavn og passord til server
	sendCredentials(sockFd);
	
	// Hvis bruker skriver GET eller PUT vil
	while(strcasecmp (inputBuffer, "quit") != 0){
		printf("Enter command <PUT> or <GET>\n");
		printf("Enter \"Quit\" to exit.\n");
		
		// Sørger for at inputBuffer er tomt når loopen kjøres
		memset(inputBuffer, 0, sizeof(inputBuffer));
		
		fgets(inputBuffer, sizeof(inputBuffer), stdin); // Brukers input
		inputBuffer[strcspn(inputBuffer, "\n")] = '\0'; // Fjerner newline
		
		// Sjekker om brukers input er gyldig kommando
        if (strcasecmp(inputBuffer, "GET") != 0 && strcasecmp(inputBuffer, "PUT") != 0 && strcasecmp(inputBuffer, "quit") != 0) {
            printf("Invalid command. Please enter <PUT>, <GET>, or \"Quit\".\n");
            continue; // Går tilbake til starten av løkken
        }
		
		if(send(sockFd, inputBuffer, strlen(inputBuffer), 0) < 0){
			printf("Couldn't send GET or PUT request\n");
			close(sockFd);
			exit(EXIT_FAILURE);
		}
		
		if(strcasecmp(inputBuffer, "GET") == 0){
			printf("<GET> write name of file\n<GET> ");
			
			fgets(inputBuffer, sizeof(inputBuffer), stdin); // Brukers input
			inputBuffer[strcspn(inputBuffer, "\n")] = '\0'; // Fjerner newline
			
			// Send brukers fileNameInput til serveren
			if(send(sockFd, inputBuffer, strlen(inputBuffer), 0) < 0){
				printf("Could not send file name\n");
				return ERROR;
			}
			
			recieveServerFile(sockFd);
			return OK;
			
		}else if(strcasecmp(inputBuffer, "PUT") == 0){
			printf("<PUT> write name of file\n<PUT> ");
			
			fgets(inputBuffer, sizeof(inputBuffer), stdin); // Brukers input
			inputBuffer[strcspn(inputBuffer, "\n")] = '\0'; // Fjerner newline
			
			sendFileToServer(sockFd, inputBuffer);
			return OK;
			
		}else{
			printf("\nCouldn't find any commands named \"%s\"\n", inputBuffer);
		}
	}
	
	close(sockFd);
	return 0;
}

int sendCredentials(int sockFd){
	char userName[BUFFERSIZE];
	char password[BUFFERSIZE];
	char response[BUFFERSIZE];
    int bytesRecieved;
	int rc = ERROR;
	
	printf("Username: ");
	fgets(userName, sizeof(userName), stdin); // Brukers input
	userName[strcspn(userName, "\n")] = '\0'; // Fjerner newline
	
	// Sender brukernavn til serveren
	if(send(sockFd, userName, strlen(userName), 0) < 0){
		printf("Could not send username\n");
		return ERROR;
	}
	
	printf("Password: ");
	fgets(password, sizeof(password), stdin); // Brukers input
	password[strcspn(password, "\n")] = '\0'; // Fjerner newline
	
	// Sender passord til serveren
	if(send(sockFd, password, strlen(password), 0) < 0){
		printf("Could not send password\n");
		return ERROR;
	}
	
	// Mottar autentiseringsrespons
    bytesRecieved = recv(sockFd, response, BUFFERSIZE, 0);
    if (bytesRecieved <= 0) {
        printf("Failed to receive authentication response\n");
        return ERROR;
    }
	response[bytesRecieved] = '\0'; // Null terminering
	printf("From server: %s\n", response);
	
	return OK;
}



