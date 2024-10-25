#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include "client.h"


// Sends txt file to server
int sendFileToServer(int sockFd, char fileNameInput[BUFFERSIZE]){
	FILE *fp;
	int rc = ERROR;
    char data[SIZE] = {0};
    char buffer[SIZE] = {0};
    char fileName[] = "message_from_client.txt";
    int bytesReceived = 0;

	if(strcasecmp(fileNameInput, fileName) != 0){
		printf("Can't find file named %s\n", fileNameInput);
		close(sockFd);
		exit(EXIT_FAILURE);
	}

    fp = fopen(fileName, "r");
    if(fp == NULL){
        printf("Error in reading file\n");
        close(sockFd);
        fclose(fp);
        exit(EXIT_FAILURE);
    }
	
	
	// Sender fileName til server
	if(send(sockFd, fileName, strlen(fileName) +1 , 0) < 0){ // +1 for null terminering
		printf("Couldn't send fileName.\n");
		return rc;
	}
	printf("Filename sent.\n");
	
	 // Mottar svar fra server
    bytesReceived = recv(sockFd, buffer, SIZE, 0);
    if(bytesReceived <= 0){
        printf("Couldn't receive server reply\n");
        close(sockFd);
        exit(EXIT_FAILURE);
    }
    printf("Server: %s", buffer);

	// Sender filinnhold til server
    while(fgets(data, SIZE, fp) != NULL){
        if(send(sockFd, data, strlen(data), 0) < 0){
            printf("Couldn't send data\n");
            fclose(fp);
            return rc;
        }else{
        	printf("Data sent to server.\n");
        }
        bzero(data, SIZE); // Nullstiller innholdet i databufferen
    }

    fclose(fp);
    rc = OK;
    return rc;
}

int recieveServerFile(int sockFd){
	FILE *fp;
	char fileName[SIZE];
	char data[SIZE];
	int bytesReceived = 0;
	char *reply;
    
    // Mottar filnavn fra server
    bytesReceived = recv(sockFd, fileName, SIZE, 0);
    if(bytesReceived <= 0){
        printf("Couldn't receive file name\n");
        close(sockFd);
        exit(EXIT_FAILURE);
    }
    
    fileName[bytesReceived] = '\0'; // Null terminerer filnavnet
	bytesReceived = 0; // For å klarere input før den brukes på ny
	
    printf("Receiving filename: %s\n", fileName);
    
    reply = "filename recieved\n";
    
    if(send(sockFd, reply, strlen(reply), 0) < 0){
		printf("Could not send message...\n");
		return ERROR;
	}else{
		printf("Reply sent\n");
	}
	
    printf("Waiting for file data...\n");
    
	fp = fopen(fileName, "w");
    if(fp == NULL){
        printf("Error in opening file.\n");
        close(sockFd);
        exit(EXIT_FAILURE);
    }
    
    while(bytesReceived = recv(sockFd, data, SIZE, 0) > 0){
		
	    fprintf(fp, "%s", data);
        bzero(data, SIZE); // Nullstiller innholdet i databufferen
    }
    if(bytesReceived < 0){
		    printf("Couldn't receive data\n");
		    close(sockFd);
		    exit(EXIT_FAILURE);
    	}else{
    	    printf("File data recieved\n");
    	}
    
    fclose(fp);
    return OK;
}
