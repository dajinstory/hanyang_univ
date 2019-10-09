#include "types.h"
#include "user.h"
int
main(int argc, char* argv[])
{
    int idx, pid, loop = 23;
    pid=fork();
    for(idx=0;idx<loop;idx++){
        if(pid==0){
            printf(1, "Child\n");
            yield();
        }
        else if(pid>0){
            printf(1, "Parent\n");
            yield();
        }
        else{
            printf(1, "fork error\n");
        }
    }
    if(pid>0) wait();
    exit();
}
