#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 4096
#define PORT_NUM 8654


int main(int argc, char *argv[])
{
	int receive_sd;
	FILE *fp;	
	
	char receive_buffer[BUFFER_SIZE];
	int read_cnt;
	struct sockaddr_in server_addr;

	if(argc!=2) {
		printf("Usage: %s <SERVER IP> \n", argv[0]);
		exit(1);
	}
	
	fp=fopen("receive.dat", "wb");	
	receive_sd=socket(PF_INET, SOCK_STREAM, 0);   

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr(argv[1]);
	server_addr.sin_port=htons(PORT_NUM);

	
	connect(receive_sd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	

	while((read_cnt=read(receive_sd, receive_buffer, BUFFER_SIZE ))!=0)	
		fwrite((void*)receive_buffer, 1, read_cnt, fp);
	
	puts("Received file data");	
	write(receive_sd, "COMPLETE", 10);
	fclose(fp);	
	close(receive_sd);	
	return 0;
}

