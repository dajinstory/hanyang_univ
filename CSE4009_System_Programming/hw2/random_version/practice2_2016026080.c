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
int arr_size, count=1, random_number;

void *t_function1(void* data){
	char* thread_name = (char*)data;
	int i,j;

	//lock
	pthread_mutex_lock(&mutex_lock);
	
	//critical section
	for(i=arr_size; i<arr_capacity && i<arr_size+random_number; i++){
		arr[i]=count++;
		for(j=arr_size;j<=i;j++){
			printf("%d,",arr[j]);
		}
		printf("\n");
	}
	arr_size=i;

	usleep(10000);	
	//unlock
	pthread_mutex_unlock(&mutex_lock);
}

void *t_function2(void* data){
	char* thread_name = (char*)data;
	int i,j;


	//lock
	pthread_mutex_lock(&mutex_lock);

	//critical section
	for(i=0; i<arr_size; i++){
		printf("popped %d\n", arr[i]);
	}
	arr_size=0;

	//unlock
	pthread_mutex_unlock(&mutex_lock);
}

int main(){
	int status;
	pthread_t p_thread1, p_thread2;

	//뮤텍스 객체를 초기화, 기본속성으로 만든다.
	pthread_mutex_init(&mutex_lock, NULL);
	
	//get random
	srand(time(NULL));

	while(1){
		//get random_number
		random_number=rand()%arr_capacity+1;
		pthread_create(&p_thread1, NULL, t_function1,(void*)"Thread1");
		pthread_create(&p_thread2, NULL, t_function2,(void*)"Thread2");
		
		pthread_join(p_thread1, (void*)&status);
		pthread_join(p_thread2, (void*)&status);	
	}

	return 0;
}
