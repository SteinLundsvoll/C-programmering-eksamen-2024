#ifndef SERVER_H
#define SERVER_H

#define OK		0
#define ERROR	1

#define BUFFERSIZE	256
#define SIZE 1024

int ftp_check_user(char *pszUsername, char *pszPassword);

int authenticateClient(int new_socket);

int sendFileToClient(int new_socket, char fileNameInput[BUFFERSIZE]);

int recieveClientFile(int new_socket);

#endif //SERVER_H
