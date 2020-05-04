#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
#define PORT 9998
 
int main(void){
    int sock;
    struct sockaddr_in target_addr;
    char recv_buffer[1024];
    int recv_len;
    int addr_len;
    const char *msg = "hello network!";
    
    if((sock = socket(AF_INET, /*3번째 인자가 0일때 UDP가 기본으로 작동할수있는 옵션!*/, 0)) < 0){
        perror("socket ");
        return 1;
    }
 
    memset(&target_addr, 0x00, sizeof(target_addr));
    target_addr.sin_family = /*주소형식이 IPv4일때*/;
    target_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    target_addr.sin_port = htons(PORT);
 
    addr_len = sizeof(target_addr);
 
    /*비 연결지향형 통신에서 데이터그램을 전송할때 쓰는 함수*/
 
    if((recv_len = /*비 연결지향형 통신에서 데이터그램을 전송받을때 쓰는 함수*/ < 0){
        fprintf(stderr,"recvfrom()")
        exit(2)
    }
    
    recv_buffer[recv_len] = '\0';
 
    printf("ip : %s\n", inet_ntoa(target_addr.sin_addr));
    printf("received data : %s\n", recv_buffer);
 
    close(sock);
 
    return 0;


