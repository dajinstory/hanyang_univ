#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#define MAX_INPUT 100000
#define MAX_ORDER 50000
#define MAX_CMD 10000


//getInput = get the input(both in batch mode and in interactive mode). distinguished by '\n'
//parseInput = parse the input data to each order. order means the operations divide by the token ';'
//parseOrder = parse the order. divide one operation by the token ' '. and save it at cmd[].

int getInput(char input[], FILE* fp);
int parseInput(char input[], char* order[], int* num_of_order);
int parseOrder(char* order, char* cmd[], int* num_of_cmd);

//helper. print the command
void print_cmd(char* cmd[], int num_of_cmd);

int main(int argv, char* argc[]){
    
    char input[MAX_INPUT+2];                        //input string
    char* order[MAX_ORDER];                         //order after parsing input by ';'
    char* cmd[MAX_CMD];                             //command after parsing order by ' '
    FILE *fp = NULL;                                //input file pointer (batch = fp, interactive = stdin)
    int num_of_order = 0;                           //number of orders after parsing input
    int num_of_cmd=0;                               //number of cmds for one orders
    pid_t pid;                                      //save the pid from fork()
    pid_t pids[MAX_ORDER];                          //array for managing child process
    int status, input_signal;               
    
    
    if(argv>1)fp=fopen(argc[1], "r");               //link fp to the file.
    else if(argv>2){                                //too much files
        printf("[ERROR] too many argument\n");
    }

    //main loop
    while(1){
        num_of_order=0;

        //get input
        input_signal = getInput(input, fp);
        if(input_signal == -1){
            printf("[ERROR] error in input operation\n");
            exit(0);
        } 
        else if(input_signal==0) {
            exit(0);
        }

        //parse input to orders
        if(parseInput(input, order, &num_of_order) == -1){
            printf("[ERROR] error in parsing input\n");
            exit(0);
        }
        


        //execute each orders
        //printf("\n---------------------------num_of_orders = %d\n", num_of_order);
        for(int i=0;i<num_of_order;i++){
            pids[i]=0;
            num_of_cmd=0;

            //parse order to exact command
            if(parseOrder(order[i], cmd, &num_of_cmd)==-1){
                printf("[ERROR] error in parsing order\n\n");
                continue;
            }
            
            //check command
            if(fp!=NULL){
                print_cmd(cmd, num_of_cmd);
            }
            //make sure it is a valid command
            if(num_of_cmd==0){
                printf("[WARNING] no valid order\n\n");
                continue;
            }


            //exit function
            if(num_of_order==1 && num_of_cmd==1 && strcmp(cmd[0],"quit")==0){
                return 0;
            }

            //execute child process
            pid = fork();
            if(pid==-1){
                printf("[ERROR] error in fork process\n\n");
                exit(0);
            }
            else if(pid==0){

                if(execvp(cmd[0], cmd)==-1){
                    printf("[ERROR] no such command\n");
                }
                exit(0);
            }
            else{
                pids[i]=pid;
            }
        }
        for(int i=0;i<num_of_order;i++){ 
            if(pids[i]!=0){
                waitpid(pids[i], &status, 0);
            }
        }

    }
    if(fp!=NULL) fclose(fp);
	return 0;
}

int getInput(char input[], FILE* fp){

    for(int i=0;i<10;i++)input[i]='\0';
    //interactie mode
    if(fp==NULL){
        printf("\nprompt> ");
        if(fgets(input, MAX_INPUT, stdin)==NULL){
            return -1;
        }
    }
    //batch mode
    else{
        //fgets error
        if(fgets(input, MAX_INPUT, fp)==NULL){
            if(feof(fp)==0){
                return -1;
            }
            else{
                return 0;
            }
        }
    }
    int len=strlen(input);

    input[len]='\0';
    input[len-1]=';';
    return 1;
}

int parseInput(char input[], char* order[], int* num_of_order){
    int idx=0, len=strlen(input), address=0;
    for(int i=0;i<len;i++){
        if(input[i]==';'){
            input[i]='\0';
            order[idx]=input+address;

            address=i+1;
            idx++;
        }
    }
    *num_of_order=idx;
    return 1;
}


int parseOrder(char* order, char* cmd[], int* num_of_cmd){
    int idx = 0;
    for(cmd[idx]=strtok(order, " "); cmd[idx]!=NULL;cmd[++idx]=strtok(NULL, " "));
    *num_of_cmd=idx; 
    cmd[idx]='\0';
    return 1;
}


void print_cmd(char* cmd[], int num_of_cmd){
    //printf("\n-------------num_of_cmd = %d\n\n", num_of_cmd);
            
    //print command
    printf("command = (");
    for(int j=0;j<num_of_cmd;j++){
        printf("%s ", cmd[j]);
    }
    printf(")\n");
}

