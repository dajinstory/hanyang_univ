#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
#define PORT 9000
 
int main(void){
    int sock;
    struct sockaddr_in addr;
    char buffer[1024];
    const char *msg = "hello! world!";
    int recv_len;
 
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
       fprintf(stderr,"socket() error");
        exit(3);
    }
 
    memset(&addr, 0x00, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(PORT);
 
    if(/*연결 요청을 보내어 서버에 연결을 신청한다*/ < 0){
        fprintf(stderr,"connect() error");
        exit(3);
    }
 
    if(/*데이터를 보낸다*/ < 0){
       fprintf(stderr,"send() error");
        exit(3);
    }
    if((recv_len = /*연결된 소켓으로 데이터를 받아 버퍼에 저장한다*/(sock, buffer, 1024, 0)) < 0){
       fprintf(stderr,"recv() error");
        exit(3);
    }
 
    buffer[recv_len] = '\0';
 
    printf("recevied data : %s\n", buffer);
 
    close(sock);
 
    return 0;
}
