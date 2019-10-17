#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <algorithm>
#define BUFF_SIZE 1024

using namespace std;

void string_to_lower(char *str);

int main(int argc, char* argv[]){
	FILE *fp;
	string command = "git ls-files `git rev-parse --show-toplevel`";
	char buff[BUFF_SIZE];

	if(argc<2){
		printf("no search key!\n");
		exit(1);
	}

	string_to_lower(argv[1]);
	fp=popen(command.c_str(), "r");
	while(fgets(buff, BUFF_SIZE, fp)!=NULL){
		char buff_lower[BUFF_SIZE];
		int buff_length=strlen(buff);
		strcpy(buff_lower, buff);
		string_to_lower(buff_lower);
		if(strstr(buff_lower, argv[1])){
			printf("%s", buff);
		}
	}
	return 0;
}

void string_to_lower(char *str){
	int index, length=strlen(str);
	for(index=0;index<length;index++){
		*(str+index)=_tolower(*(str+index));
	}
}
