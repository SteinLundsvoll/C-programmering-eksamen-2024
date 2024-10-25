#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "server.h" 


int sendFileToClient(int new_socket, char fileNameInput[BUFFERSIZE]){
	FILE *fp;
	int rc = ERROR;
    char data[SIZE] = {0};
    char buffer[SIZE] = {0};
    char fileName[] = "message_from_server.txt";
    int bytesReceived = 0;

	if(strcasecmp(fileNameInput, fileName) != 0){
		printf("Can't find file named %s\n", fileNameInput);
		close(new_socket);
		exit(EXIT_FAILURE);
	}

    fp = fopen(fileName, "r");
    if(fp == NULL){
        printf("Error in reading file\n");
        close(new_socket);
        fclose(fp);
        exit(EXIT_FAILURE);
    }
	
	
	// Sender fileName til client
	if(send(new_socket, fileName, strlen(fileName) +1 , 0) < 0){ // +1 for null terminering
		printf("Couldn't send fileName.\n");
		return rc;
	}
	printf("Filename sent.\n");
	
	 // Mottar svar fra client
    bytesReceived = recv(new_socket, buffer, SIZE, 0);
    if(bytesReceived <= 0){
        printf("Couldn't receive client reply\n");
        close(new_socket);
        exit(EXIT_FAILURE);
    }
    printf("Server: %s", buffer);

	// Sender filinnhold til client
    while(fgets(data, SIZE, fp) != NULL){
        if(send(new_socket, data, strlen(data), 0) < 0){
            printf("Couldn't send data\n");
            fclose(fp);
            return rc;
        }else{
        	printf("Data sent to client.\n");
        }
        bzero(data, SIZE); // Nullstiller innholdet i databufferen
    }

    fclose(fp);
    rc = OK;
    return rc;
}

int recieveClientFile(int new_socket){
	FILE *fp;
	char fileName[SIZE];
	char data[SIZE];
	int bytesReceived = 0;
	char *reply;
    
    // Mottar filnavn fra klient
    bytesReceived = recv(new_socket, fileName, SIZE, 0);
    if(bytesReceived <= 0){
        printf("Couldn't receive file name\n");
        close(new_socket);
        exit(EXIT_FAILURE);
    }
    
    fileName[bytesReceived] = '\0'; // Null terminerer filnavnet
	bytesReceived = 0; // For å klarere input før den brukes på ny
	
    printf("Receiving filename: %s\n", fileName);
    
    reply = "filename recieved\n";
    
    if(send(new_socket, reply, strlen(reply), 0) < 0){
		printf("Could not send message...\n");
		return ERROR;
	}else{
		printf("Reply sent\n");
	}
	
    printf("Waiting for file data...\n");
    
	fp = fopen(fileName, "w");
    if(fp == NULL){
        printf("Error in opening file.\n");
        close(new_socket);
        exit(EXIT_FAILURE);
    }
    
    while(bytesReceived = recv(new_socket, data, SIZE, 0) > 0){
		
	    fprintf(fp, "%s", data);
        bzero(data, SIZE); // Nullstiller innholdet i databufferen
    }
    if(bytesReceived < 0){
		    printf("Couldn't receive data\n");
		    close(new_socket);
		    exit(EXIT_FAILURE);
    	}else{
    	    printf("File data recieved\n");
    	}
    
    fclose(fp);
    return OK;
}
