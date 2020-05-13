#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
#define PORT 9000
 
int main(void){
    int sock, client_sock;
    char buffer[1024];
    int len, addr_len, recv_len;
 
    //if((sock = /*소켓을 생성한다.*/)<0){ //create socket!
    if((sock = socket(AF_INET, SOCK_STREAM, 0))<0){ //create socket!
        fprintf(stderr,"socket error");
        exit(1);
    }

    struct sockaddr_in addr, client_addr;
    memset(&addr, 0x00, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);
 
    //if(/*소켓 디스크립터와 주소정보를 묶어준다*/ < 0){
    if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        perror("bind ");
        exit(2);
    }
 
    //if(/*클라이언트로부터의 요청을 청취한다*/ < 0){
    if(listen(sock, 5) < 0){
        perror("listen ");
		exit(3);
    }
 
    addr_len = sizeof(client_addr);
 
    printf("waiting for client..\n");
 
    //while((client_sock = /*연결요청에따라 연결을 수립하고, 
    //연결된 클라이언트와 통신을위한 소켓을 생성한다*/ > 0){
    while((client_sock = accept(sock, (struct sockaddr *)&client_addr, &addr_len)) > 0){
        printf("client ip : %s\n", inet_ntoa(client_addr.sin_addr));
        
        if((recv_len = recv(client_sock, buffer, 1024, 0)) < 0){
            perror("recv ");
            return 1;
            break;
        }
        buffer[recv_len] = '\0';
        printf("received data : %s\n", buffer);
        send(client_sock, buffer, strlen(buffer), 0);
 
        close(client_sock);
    }
    close(sock);
    return 0;
}
