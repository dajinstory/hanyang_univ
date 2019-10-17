#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#define arr_capacity 40

//뮤텍스 객체를 선언한다.
pthread_mutex_t mutex_lock;

int arr[arr_capacity];
int arr_size, count=1;

void *t_function1(void* data){
	char* thread_name = (char*)data;
	int i;
	while(1){
		while(arr_size<arr_capacity){
			//thread1 도중에 thread2가 실행되는 것은 괜찮다.
			//단 arr[arr_size++]=count++ 가 실행되고, 현재상황이 출력되는 그 사이에 실행되는것을 막기 위해 mutex_lock을 쓴다.

			pthread_mutex_lock(&mutex_lock);
			
			arr[arr_size++]=count++;
			for(i=0; i<arr_size; i++){
				printf("%d,",arr[i]);
			}
			printf("\n");

			pthread_mutex_unlock(&mutex_lock);
		}
	}
}

void *t_function2(void* data){
	char* thread_name = (char*)data;
	int i;

	while(1){
		//lock
		pthread_mutex_lock(&mutex_lock);

		//critical section
		for(i=0; i<arr_size; i++){
			printf("popped %d\n", arr[i]);
		}
		arr_size=0;
	
		//unlock
		pthread_mutex_unlock(&mutex_lock);
		usleep(10);
	}
}
int main(){
	int status;
	pthread_t p_thread1, p_thread2;

	//뮤텍스 객체를 초기화, 기본속성으로 만든다.
	pthread_mutex_init(&mutex_lock, NULL);
	
	//두 스레드를 실행
	pthread_create(&p_thread1, NULL, t_function1,(void*)"Thread1");
	pthread_create(&p_thread2, NULL, t_function2,(void*)"Thread2");

	pthread_join(p_thread1, (void*)&status);
	pthread_join(p_thread2, (void*)&status);	

	return 0;
}
