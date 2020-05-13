// Linux
// ������ �ɼ� -pthread �߰�
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include <pthread.h>

#define MAX_BUFF 100
#define MAX_MESSAGE_LEN 256

typedef struct Message {
	int user_id;
	char str[MAX_MESSAGE_LEN];
}Message;

void *sendThread();
void *recvThread(void *data);
void *sendThreadClient();
int isFull();
int isEmpty();
int enqueue(Message item);
Message* dequeue();

int sock_main, sock_client[10];
Message *msgbuff;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int front = -1;
int rear = -1;

int main()
{
	int mode;
	int count = 0;
	int th_id;
	Message buff;
	pthread_t th_send;

	struct sockaddr_in addr;

	printf("1. Server, 2. Client: ");
	scanf("%d", &mode);
	getchar();

	if (mode == 1) {
		pthread_t th_recv[10];

		msgbuff = (Message *)malloc(sizeof(Message) * MAX_BUFF);

		// Create Send Thread
		th_id = pthread_create(&th_send, NULL, sendThread, 0);

		//1번 공백
		// NIC에 존재하는 모든 주소로
		// 포트 번호 : 36007
		addr.sin_family = AF_INET;
		// here

		//2번 공백
		// 서버 소켓 생성, 조건 : IPv4 TCP
		// here

		// 3번 공백
		// 서버 소켓으로 주소 bind
		// here

		// 4번 공백
		// 연결 청취 ,queue는 5개
		// here

		while (1) {
			// 5번 공백 연결 수립
			// here

			// client thread
			if (count < 10) {
				int idx = count;
				th_id = pthread_create(&th_recv[count], NULL, recvThread, (void *)&idx);
				count++;
			}
		}
	}
	else {

		//6번 공백 주관식!(1번하고 주소 파츠만 다릅니다)
		// IP 주소 127.0.0.1 포트 번호 36007
		addr.sin_family = AF_INET;
		// here

		// 2번 공백 두번째(답이 똑같아요...)
		// 소켓 생성, IPv4 TCP
		// here

		// 7번 공백 연결 요청
		// here

		// Client Send Thread
		th_id = pthread_create(&th_send, NULL, sendThreadClient, 0);

		while (1) {

			//8번 공백 
			//recv받은 값의 크기가 0보다 크다면 출력
			memset(&buff, 0, sizeof(buff));

			if (/*here*/) > 0) {
				printf("User %d: %s\n", buff.user_id, buff.str);
			}
			else {
				printf("Disconnected\n");
				exit(5);
			
		}
	}

	return 0;
}


void *sendThread() {
	Message *tmp;

	printf("Send Thread Start\n");

	while (1) {
		//9번공백
		//dequeue 함수의 실행 결과가 NULL(Queue Empty)가 아니라면 전송
			if ((tmp = dequeue()) != NULL) {
			for (int i = 0; i < 10; i++) {
				if (i != tmp->user_id) {
					// here
				}
			}
		}

		

		usleep(1000);
	}
}

void *recvThread(void *data) {
	Message buff;
	int thread_id = *((int*)data);

	printf("Receive Thread %d Start\n", thread_id);

	//10번 공백
	// 클라이언트로부터 수신한 결과가 0보다 크면 enqueue
	while (/*here*/) {
		buff.user_id = thread_id;
		if (enqueue(buff) == -1) {
			printf("Messag Buffer Full\n");
}

void *sendThreadClient() {
	Message tmp;
	int count;

	while (1) {

		memset(&tmp, 0, sizeof(tmp));
		fgets(tmp.str, MAX_MESSAGE_LEN, stdin);
		tmp.str[strlen(tmp.str) - 1] = '\0';
		tmp.user_id = -1;

		count = send(sock_main, (char *)&tmp, sizeof(Message), 0);
	}
}

int isFull() {
	if ((front == rear + 1) || (front == 0 && rear == MAX_BUFF - 1)) {
		return 1;
	}
	return 0;
}

int isEmpty() {
	if (front == -1) {
		return 1;
	}
	return 0;
}

int enqueue(Message item) {

	if (isFull()) {
		return -1;
	}
	else {
		pthread_mutex_lock(&mutex);
		if (front == -1) {
			front = 0;
		}
		rear = (rear + 1) % MAX_BUFF;
		msgbuff[rear].user_id = item.user_id;
		strcpy(msgbuff[rear].str, item.str);
		pthread_mutex_unlock(&mutex);
	}
	return 0;
}

Message* dequeue() {
	Message *item;

	if (isEmpty()) {
		return NULL;
	}
	else {
		pthread_mutex_lock(&mutex);
		item = &msgbuff[front];

		if (front == rear) {
			front = -1;
			rear = -1;
		}
		else {
			front = (front + 1) % MAX_BUFF;
		}
		pthread_mutex_unlock(&mutex);
		return item;
	}

}
