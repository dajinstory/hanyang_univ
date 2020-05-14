// linux
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>

void ConnectToServer();
void *recvDataThread();
void passiveMode();

int mode, port;
char buff[1024];
char ip_address[16];
char local_ip_address[16];
char username[31];
char password[31];
char command[100];
int remote_ip[4];
int remote_port[2];

FILE *fp;

int FileDownMode = 0;
int transferMode = 0; // 0 ascii, 1 binary

int sock_main;

struct sockaddr_in addr, addr_data;

int main() {
	char tmp[40] = { 0 };

	printf("1. Server, 2. Client: ");
	scanf("%d", &mode);
	getchar();

	if (mode == 1) {

	}
	else if (mode == 2) {
		printf("IP_ADDR: ");
		scanf("%s", ip_address);
		printf("PORT_NUM: ");
		scanf("%d", &port);
		printf("User name: ");
		scanf("%s", username);
		printf("password: ");
		scanf("%s", password);
		getchar();

		ConnectToServer();

		while (1) {
			printf("ftp> ");
			memset(command, 0, sizeof(command));
			fgets(command, 1000, stdin);
			command[strlen(command) - 1] = '\0';
			memset(tmp, 0, sizeof(tmp));

			if (strcmp(command, "quit") == 0) {
				sprintf(tmp, "QUIT \r\n");
			}
			else if (strcmp(command, "ls") == 0) {
				strcpy(tmp, "NLST\r\n");
				passiveMode();
				pthread_t th_recv;
				pthread_create(&th_recv, NULL, recvDataThread, 0);
			}
			else if (strncmp(command, "cd ", 3) == 0) {
				char *tmp2 = strtok(command, " ");
				tmp2 = strtok(NULL, " ");
				sprintf(tmp, "CWD %s\r\n", tmp2);
				passiveMode();
				pthread_t th_recv;
				pthread_create(&th_recv, NULL, recvDataThread, 0);
			}
			else if (strncmp(command, "get ", 4) == 0) {
				char *tmp2 = strtok(command, " ");
				tmp2 = strtok(NULL, " ");
				FileDownMode = 1;
				
				if (transferMode == 0)
					fp = fopen(tmp2, "wt");
				else fp = fopen(tmp2, "wb");

				sprintf(tmp, "RETR %s\r\n", tmp2);
				passiveMode();
				pthread_t th_recv;
				pthread_create(&th_recv, NULL, recvDataThread, 0);
			}
			else if (strcmp(command, "bin") == 0) {
				strcpy(tmp, "TYPE I \r\n");
			}
			else if (strcmp(command, "ascii") == 0) {
				strcpy(tmp, "TYPE A \r\n");
			}
			else {
				printf("Commands: ls, cd, get..\n");
				continue;
			}

			send(sock_main, tmp, strlen(tmp), 0);

			memset(buff, 0, sizeof(buff));

			while (recv(sock_main, (char*)buff, sizeof(buff), 0) > 0) {
				printf("%s", buff);

				if ((strncmp(buff, "226", 3) == 0) || (strncmp(buff, "250", 3) == 0) || (strncmp(buff, "425", 3) == 0) || (strncmp(buff, "211", 3) == 0) || (strncmp(buff, "200", 3) == 0)) {
					break;
				}
				else if (strncmp(buff, "221", 3) == 0) {
					close(sock_main);
					exit(10);
				}
				memset(buff, 0, sizeof(buff));
			}
		}
	}
	else {
		printf("DISCONNECT\n");
	}

	return 0;
}

void ConnectToServer() {
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip_address);

	if ((sock_main = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("Socket Open Failed\n");
		exit(1);
	}

	if (connect(sock_main, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		printf("Connect Failed\n");
		exit(4);
	}

	while (1) {
		memset(buff, 0, sizeof(buff));

		if (recv(sock_main, (char*)buff, sizeof(buff), 0) > 0) {
			printf("%s", buff);

			if (strncmp(buff, "220", 3) == 0) {
				char tmp[40] = { 0 };
				sprintf(tmp, "USER %s\r\n", username);
				send(sock_main, tmp, strlen(tmp), 0);
			}
			else if (strncmp(buff, "331", 3) == 0) {
				char tmp[40] = { 0 };
				sprintf(tmp, "PASS %s\r\n", password);
				send(sock_main, tmp, strlen(tmp), 0);
			}
			else if (strncmp(buff, "230", 3) == 0) {
				return;
			}
		}
		else {
			printf("Disconnected\n");
			close(sock_main);
			exit(5);
		}
	}
}

void *recvDataThread() {
	int sock_data;
	addr_data.sin_family = AF_INET;
	addr_data.sin_port = htons(((unsigned char)remote_port[0] * 256) + (unsigned char)remote_port[1]);
	addr_data.sin_addr.s_addr = htonl((unsigned char)remote_ip[0] << 24 | (unsigned char)remote_ip[1] << 16 | (unsigned char)remote_ip[2] << 8 | (unsigned char)remote_ip[3]);

	if ((sock_data = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("Socket Open Failed\n");
		exit(1);
	}

	if (connect(sock_data, (struct sockaddr*)&addr_data, sizeof(addr_data)) == -1) {
		printf("Connect Failed\n");
		exit(4);
	};

	int flags = 1;
	if (setsockopt(sock_main, SOL_SOCKET, SO_KEEPALIVE, (void*)&flags, sizeof(flags)) == -1) printf("setsockopt1 failed\n");

	char tmpbuff[1460];

	memset(tmpbuff, 0, sizeof(tmpbuff));

	int count = 0;

	while ((count = recv(sock_data, (char*)tmpbuff, sizeof(tmpbuff), 0)) > 0) {
		if (FileDownMode == 1) {
			if (transferMode == 1)
				fwrite(tmpbuff, sizeof(char), count, fp);
			else fprintf(fp, "%s", tmpbuff);
		}
		else {
			printf("%s", tmpbuff);
		}
		memset(tmpbuff, 0, sizeof(tmpbuff));
	}

	if (FileDownMode == 1) {
		FileDownMode = 0;
		fclose(fp);
	}
	close(sock_data);
}

void passiveMode() {
	char tmp[40] = { 0 };

	sprintf(tmp, "PASV \r\n");
	printf("%s", tmp);
	send(sock_main, tmp, strlen(tmp), 0);

	memset(buff, 0, sizeof(buff));

	while (recv(sock_main, (char*)buff, sizeof(buff), 0) > 0) {
		printf("%s", buff);

		if (strncmp(buff, "227", 3) == 0) {
			sscanf(buff, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)", &remote_ip[0], &remote_ip[1], &remote_ip[2], &remote_ip[3], &remote_port[0], &remote_port[1]);
			return;
		}
	}
}
