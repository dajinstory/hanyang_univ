#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

//getppid
int
sys_getppid(void)
{
    return myproc()->parent->pid;
}

//yield
int
sys_yield(void)
{
    yield();
    return 0;
}

int
sys_cpu_share(void)
{
    int n;
    if(argint(0,&n)<0) return -1;
    cpu_share(n);
    return 0;
}

int
sys_run_MLFQ(void)
{
    run_MLFQ();
    return 0;
}

int
sys_getlev(void)
{
    return getlev();
}

int
sys_thread_create(void)
{
    int function, arg;
    //if(argint(0, &thread)<0) return -1;
    thread_t* thread;
    if(argptr(0, (void*)&thread, sizeof(*thread))<0) return -1;
    if(argint(1, &function)<0) return -1;
    if(argint(2, &arg)<0) return -1;
    
    if(thread_create((thread_t*)thread, (void*((*)(void*)))function, (void*)arg)==-1){
        return -1;
    }
    return 0;
}


int
sys_thread_exit(void)
{
    int retval;
    if(argint(0, &retval)<0)return -1;
    thread_exit((void*)retval);
    return 0;
}

int
sys_thread_join(void)
{
    int retval;
    //if(argint(0, &thread)<0) return -1;
    thread_t* thread;
    if(argptr(0, (void*)&thread, sizeof(*thread))<0) return -1;
    if(argint(1, &retval)<0) return -1;
    if(thread_join((thread_t*)thread, (void**)retval)==-1){
        return -1;
    }
    return 0;
}

int
sys_sync(void)
{
    if(sync()==-1)return -1;
    return 0;
}

int
sys_get_log_num()
{
    return get_log_num();
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
