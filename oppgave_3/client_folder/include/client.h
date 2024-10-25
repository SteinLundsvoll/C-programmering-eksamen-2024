#ifndef CLIENT_H
#define CLIENT_H

#define OK		0
#define ERROR	1
#define BUFFERSIZE	256
#define SIZE 1024

int sendCredentials(int sockFd);

int sendFileToServer(int sockFd, char fileNameInput[BUFFERSIZE]);

int recieveServerFile(int sockFd);

#endif //CLIENT_H
