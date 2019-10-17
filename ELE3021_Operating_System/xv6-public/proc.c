#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

typedef struct node{
    int ticks;
    int idx;
}node;
//queue structure
typedef struct queue{
    int first;
    int end;
    int size;
    int tick_quantum;
    int tick_allotment;
    node elements[NPROC];
} queue;


int queue_size(queue *myQ){
    return myQ->size;
}
node queue_front(queue *myQ){
    if(myQ->size==0){
        panic("Queue is empty\n");
        return myQ->elements[myQ->first];
    }
    return myQ->elements[myQ->first];
}
void queue_pop(queue *myQ){
    if(myQ->size==0){
        panic("Queue has no elements\n");
        return;
    }
    myQ->size--;
    myQ->first = (myQ->first + 1) % NPROC;
}

void queue_push(queue *myQ, node newProc){
    if(myQ->size==NPROC){
        panic("Queue is full\n");
        return;
    }
    myQ->size++;
    myQ->elements[myQ->end] = newProc;
    myQ->end = (myQ->end + 1) % NPROC;
}

void MLFQ_boost(queue *q2, queue *q0){
    while(queue_size(q2)!=0){
        node front = queue_front(q2);
        queue_pop(q2);
        front.ticks=0;
        queue_push(q0, front);
    }
}

struct {
  struct spinlock lock;
  struct proc proc[NPROC];
  int retval[NPROC];
  int totalStride;
  
  int stride[NPROC]; //[NPROC] = TotalMLFQ. 0:MLFQ, 1~N: Stride
  float distance[NPROC];
  int ticks[NPROC];

  int MLFQ_level;
  int MLFQ_procTicks;
  int MLFQ_procIdx;
  int MLFQ_restTicks;
  int MLFQ_stride;
  float MLFQ_distance;
  queue q[3];
} ptable;


static struct proc *initproc;

int nextpid = 1;
extern void forkret(void);
extern void trapret(void);

static void wakeup1(void *chan);

void
pinit(void)
{
  initlock(&ptable.lock, "ptable");
}

// Must be called with interrupts disabled
int
cpuid() {
  return mycpu()-cpus;
}

// Must be called with interrupts disabled to avoid the caller being
// rescheduled between reading lapicid and running through the loop.
struct cpu*
mycpu(void)
{
  int apicid, i;
  
  if(readeflags()&FL_IF)
    panic("mycpu called with interrupts enabled\n");
  
  apicid = lapicid();
  // APIC IDs are not guaranteed to be contiguous. Maybe we should have
  // a reverse map, or reserve a register to store &cpus[i].
  for (i = 0; i < ncpu; ++i) {
    if (cpus[i].apicid == apicid)
      return &cpus[i];
  }
  panic("unknown apicid\n");
}

// Disable interrupts so that we are not rescheduled
// while reading proc from the cpu structure
struct proc*
myproc(void) {
  struct cpu *c;
  struct proc *p;
  pushcli();
  c = mycpu();
  p = c->proc;
  popcli();
  return p;
}

int getProcIdx(void){
    int idx;
    struct proc *p;
    struct proc *myProc = myproc();
    for(;;){
        for(idx=0, p=ptable.proc; p < &ptable.proc[NPROC]; idx++, p++){
            if (p->pid == myProc->pid){
                return idx;
            }
        }
    }
    panic("no process");
    return -1;
}

int getProcIdx2(struct proc* myProc){
    
    int idx;
    struct proc *p;
    for(;;){
        for(idx=0, p=ptable.proc; p < &ptable.proc[NPROC]; idx++, p++){
            if (p->pid == myProc->pid){
                return idx;
            }
        }
    }
    panic("no process");
    return -1;
}

int getProcIdx3(int procPid){
    
    int idx;
    struct proc *p;
    for(;;){
        for(idx=0, p=ptable.proc; p < &ptable.proc[NPROC]; idx++, p++){
            if (p->pid == procPid){
                return idx;
            }
        }
    }
    panic("no process");
    return -1;
}





float getMinPass(void){
    int idx;
    float minPass=200000000.0;
    float check=minPass;
    struct proc *p;
    int cnt=0;
    for(;;){
        cnt=0;
        for(idx=0, p=ptable.proc; p < &ptable.proc[NPROC]; idx++, p++){
            if (p->state != RUNNABLE) continue;
            cnt++;
            if(ptable.stride[idx]>0 && ptable.distance[idx]<minPass){
                minPass = ptable.distance[idx];
            }
        }
        if (ptable.MLFQ_stride>0 && ptable.MLFQ_distance<minPass){
            minPass = ptable.MLFQ_distance;
        }
        if(minPass<check) return minPass;
        else{
            return 0.0;
        }
    }
    return minPass;
}

int cpu_share(int portion){
   acquire(&ptable.lock);
   
    if(ptable.totalStride+portion>100){
        release(&ptable.lock);
        return -1;
    }

   int procIdx=getProcIdx();
   float minPass = getMinPass();
   //ptable.totalStride += portion-ptable.stride[procIdx];
   ptable.stride[procIdx]=portion;
   ptable.distance[procIdx]=minPass;
   
   release(&ptable.lock);
   return 0;
}

int run_MLFQ(void){
    acquire(&ptable.lock);
    
    int procIdx=getProcIdx();
    float minPass = getMinPass();    

    if(ptable.MLFQ_stride==0 && ptable.totalStride+20>100){
        release(&ptable.lock);
        return -1;
    }
    //ptable.totalStride-=ptable.stride[procIdx];
    ptable.stride[procIdx]=0;
    ptable.distance[procIdx]=0.0;
    
    ptable.MLFQ_stride=20;
    ptable.MLFQ_distance=minPass;
    
    node newProc;
    newProc.idx=procIdx;
    newProc.ticks=0;
    queue_push(&ptable.q[0], newProc);
    release(&ptable.lock);
    return 0;
}

int getlev(void){
    int re;
    acquire(&ptable.lock);
    re = ptable.MLFQ_level; 
    release(&ptable.lock);
    return re;
}

int getNextLev(void){
    int qlev;
    for(qlev=0;qlev<3;qlev++){
	    while(queue_size(&ptable.q[qlev])!=0){
            node first = queue_front(&ptable.q[qlev]); 
            if(ptable.stride[first.idx]>0){
                queue_pop(&ptable.q[qlev]);
                continue;
            }
            else if(ptable.proc[first.idx].state != RUNNABLE){
                ptable.stride[first.idx]=-(qlev+1);
                ptable.ticks[first.idx]=first.ticks;
                queue_pop(&ptable.q[qlev]);
                continue;
            }
            return qlev;
        }
    }
    return 3;
}







//PAGEBREAK: 32
// Look in the process table for an UNUSED proc.
// If found, change state to EMBRYO and initialize
// state required to run in the kernel.
// Otherwise return 0.
static struct proc*
allocproc(void)
{
  struct proc *p;
  char *sp;
  int idx;

  acquire(&ptable.lock);

  for(idx = 0, p = ptable.proc; p < &ptable.proc[NPROC]; idx++, p++)
    if(p->state == UNUSED)
      goto found;

  release(&ptable.lock);
  return 0;

found:
  p->state = EMBRYO;
  p->pid = nextpid++;

  release(&ptable.lock);

  // Allocate kernel stack.
  if((p->kstack = kalloc()) == 0){
    p->state = UNUSED;
    return 0;
  }
  //     cprintf("kalloc success %d\n", (int)p->kstack);
  ptable.stride[idx]=10;
  ptable.totalStride+=10;
  ptable.ticks[idx]=0;

  ptable.distance[idx]=getMinPass();
  sp = p->kstack + KSTACKSIZE;

  // Leave room for trap frame.
  sp -= sizeof *p->tf;
  p->tf = (struct trapframe*)sp;

  //cprintf("kstack and stack : %d %d\n", p->kstack, p->tf->esp);
  // Set up new context to start executing at forkret,
  // which returns to trapret.
  sp -= 4;
  *(uint*)sp = (uint)trapret;

  sp -= sizeof *p->context;
  p->context = (struct context*)sp;
  memset(p->context, 0, sizeof *p->context);
  p->context->eip = (uint)forkret;

  return p;
}

//PAGEBREAK: 32
// Set up first user process.
void
userinit(void)
{
  struct proc *p;
  extern char _binary_initcode_start[], _binary_initcode_size[];

  p = allocproc();
  
  initproc = p;
  if((p->pgdir = setupkvm()) == 0)
    panic("userinit: out of memory?");
  inituvm(p->pgdir, _binary_initcode_start, (int)_binary_initcode_size);
  p->sz = PGSIZE;
  memset(p->tf, 0, sizeof(*p->tf));
  p->tf->cs = (SEG_UCODE << 3) | DPL_USER;
  p->tf->ds = (SEG_UDATA << 3) | DPL_USER;
  p->tf->es = p->tf->ds;
  p->tf->ss = p->tf->ds;
  p->tf->eflags = FL_IF;
  p->tf->esp = PGSIZE;
  p->tf->eip = 0;  // beginning of initcode.S

  safestrcpy(p->name, "initcode", sizeof(p->name));
  p->cwd = namei("/");

  // this assignment to p->state lets other cores
  // run this process. the acquire forces the above
  // writes to be visible, and the lock is also needed
  // because the assignment might not be atomic.
  acquire(&ptable.lock);

  p->state = RUNNABLE;

  release(&ptable.lock);
}

// Grow current process's memory by n bytes.
// Return 0 on success, -1 on failure.
int
growproc(int n)
{
  uint sz;
  struct proc *curproc = myproc();

  sz = curproc->sz;
  if(n > 0){
    if((sz = allocuvm(curproc->pgdir, sz, sz + n)) == 0)
      return -1;
  } else if(n < 0){
    if((sz = deallocuvm(curproc->pgdir, sz, sz + n)) == 0)
      return -1;
  }
  curproc->sz = sz;
  switchuvm(curproc);
  return 0;
}

// Create a new process copying p as the parent.
// Sets up stack to return as if from system call.
// Caller must set state of returned proc to RUNNABLE.
int
fork(void)
{
  int i, pid;
  struct proc *np;
  struct proc *curproc = myproc();

  // Allocate process.
  if((np = allocproc()) == 0){
    return -1;
  }

  // Copy process state from proc.
  if((np->pgdir = copyuvm(curproc->pgdir, curproc->sz)) == 0){
    kfree(np->kstack);
    np->kstack = 0;
    np->state = UNUSED;
    return -1;
  }
  np->sz = curproc->sz;
  np->parent = curproc;
  *np->tf = *curproc->tf;

  // Clear %eax so that fork returns 0 in the child.
  np->tf->eax = 0;

  for(i = 0; i < NOFILE; i++)
    if(curproc->ofile[i])
      np->ofile[i] = filedup(curproc->ofile[i]);
  np->cwd = idup(curproc->cwd);

  safestrcpy(np->name, curproc->name, sizeof(curproc->name));

  pid = np->pid;

  acquire(&ptable.lock);

  np->state = RUNNABLE;

  release(&ptable.lock);

  return pid;
}

// Exit the current process.  Does not return.
// An exited process remains in the zombie state
// until its parent calls wait() to find out it exited.
void
exit(void)
{
  struct proc *curproc = myproc();
  struct proc *p;
  int fd, idx;

  if(curproc == initproc)
    panic("init exiting");

  // Close all open files.
  for(fd = 0; fd < NOFILE; fd++){
    if(curproc->ofile[fd]){
      fileclose(curproc->ofile[fd]);
      curproc->ofile[fd] = 0;
    }
  }

  begin_op();
  iput(curproc->cwd);
  end_op();
  curproc->cwd = 0;

  acquire(&ptable.lock);
  // Parent might be sleeping in wait().
  wakeup1(curproc->parent);

  // Pass abandoned children to init.
  for(idx=0, p = ptable.proc; p < &ptable.proc[NPROC]; idx++, p++){
    if(p->parent == curproc){
      p->parent = initproc;
      if(p->isThread == 1){
          p->kstack = 0;
      }
      if(p->state == ZOMBIE)
        wakeup1(initproc);
    }
  }

  // Jump into the scheduler, never to return.
  curproc->state = ZOMBIE;
  sched();
  panic("zombie exit");
}

// Wait for a child process to exit and return its pid.
// Return -1 if this process has no children.
int
wait(void)
{
  struct proc *p;
  int havekids, pid;
  struct proc *curproc = myproc();
  
  acquire(&ptable.lock);
  for(;;){
    // Scan through table looking for exited children.
    havekids = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->parent != curproc)
        continue;
      havekids = 1;
      if(p->state == ZOMBIE){
        // Found one.
        pid = p->pid;
        kfree(p->kstack);
        p->kstack = 0;
        freevm(p->pgdir);
        p->pid = 0;
        p->parent = 0;
        p->name[0] = 0;
        p->killed = 0;
        p->state = UNUSED;
        release(&ptable.lock);
        return pid;
      }
    }

    // No point waiting if we don't have any children.
    if(!havekids || curproc->killed){
      release(&ptable.lock);
      return -1;
    }

    // Wait for children to exit.  (See wakeup1 call in proc_exit.)
    sleep(curproc, &ptable.lock);  //DOC: wait-sleep
  }
}

//PAGEBREAK: 42
// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run
//  - swtch to start running that process
//  - eventually that process transfers control
//      via swtch back to the scheduler.
void
scheduler(void)
{
  struct proc *p;
  struct cpu *c = mycpu();
  c->proc = 0;

  ptable.q[0].first=0;
  ptable.q[0].end=0;
  ptable.q[0].size=0;
  ptable.q[0].tick_quantum=1;
  ptable.q[0].tick_allotment=5;
  
  ptable.q[1].first=0;
  ptable.q[1].end=0;
  ptable.q[1].size=0;
  ptable.q[1].tick_quantum=2;
  ptable.q[1].tick_allotment=10;
  
  ptable.q[2].first=0;
  ptable.q[2].end=0;
  ptable.q[2].size=0;
  ptable.q[2].tick_quantum=4;
  ptable.q[2].tick_allotment=2000000000;
          
  ptable.MLFQ_level=0;
  ptable.MLFQ_procIdx=-1;
  ptable.MLFQ_restTicks=0;
  ptable.MLFQ_stride=0;
  ptable.MLFQ_distance=0.0;

  for(;;){
    // Enable interrupts on this processor.
    sti();

    // Loop over process table looking for process to run.
    acquire(&ptable.lock);



    //get the next process by Stride Scheduling. 
    //and save the index to procIdx.
    int idx;
    int procIdx=-1;
    float procDist=2000000000.0;
    //float check=procDist;
    ptable.totalStride=0;
    for(idx = 0, p = ptable.proc; p < &ptable.proc[NPROC]; idx++, p++){
      if(p->state != RUNNABLE){
        continue;
      }
      //process that was in MLFQ. but state was not RUNNABLE. and then now it changed to RUNNABLE.
      if(ptable.stride[idx]<0){
        node newProc;
        newProc.idx=idx;
        newProc.ticks=ptable.ticks[idx];
        ptable.ticks[idx]=0;
        queue_push(&ptable.q[-ptable.stride[idx]-1], newProc);
        ptable.stride[idx]=0;
        continue;
      }
      //renew totalStride 
      ptable.totalStride+=ptable.stride[idx];
      if(ptable.stride[idx]>0 && (procIdx==-1 || ptable.distance[idx]<procDist)){
        procIdx=idx;
        procDist=ptable.distance[idx];
      }
    }
    ptable.totalStride+=ptable.MLFQ_stride;
    if(procIdx==-1 && ptable.MLFQ_stride==0){
        release(&ptable.lock);
        continue;
    }


    int q_level=3;
    if(ptable.MLFQ_stride>0){
        q_level=getNextLev();
        if(q_level==3){
            ptable.totalStride -= ptable.MLFQ_stride;
            ptable.MLFQ_stride=0;
        }
        else if(ptable.MLFQ_level == 2 && ptable.MLFQ_procTicks>ptable.q[2].tick_quantum*10){
            MLFQ_boost(&ptable.q[2], &ptable.q[0]);
            q_level=getNextLev();
        }
    }
    //the case MLFQ policy is selected
    if (q_level!=3 && ptable.MLFQ_stride>0 && ptable.MLFQ_distance<procDist){
        //MLFQ scheduling
        if(ptable.MLFQ_restTicks==0 || ptable.proc[ptable.MLFQ_procIdx].state != RUNNABLE){
            // get new proc in MLFQ policy
            //ptable.q[q_level].tick_allotment);
            node next = queue_front(&ptable.q[q_level]);
            queue_pop(&ptable.q[q_level]);
            ptable.MLFQ_procIdx=next.idx;
            ptable.MLFQ_restTicks=ptable.q[q_level].tick_quantum;
            ptable.MLFQ_level=q_level;
            next.ticks+=ptable.q[q_level].tick_quantum;
            ptable.MLFQ_procTicks=next.ticks;
            if(next.ticks>=ptable.q[q_level].tick_allotment){
                next.ticks=0;
                queue_push(&ptable.q[q_level+1], next);
            }
            else{
                queue_push(&ptable.q[q_level], next);
            }
            if(ptable.proc[next.idx].state!=RUNNABLE){
                cprintf("NONONONONOT RUNNABLE");
            }

        }
        procIdx=ptable.MLFQ_procIdx;
        ptable.MLFQ_restTicks--;
        ptable.MLFQ_distance += (float)(100)/(float)(ptable.MLFQ_stride);
    }
    else{
        //Stride Scheduling
        ptable.distance[procIdx] += (float)(100)/(float)(ptable.stride[procIdx]);
    }

    p = ptable.proc+procIdx;
    if(p->kstack==0 && procIdx==ptable.MLFQ_procIdx){
        if(p->state==UNUSED){
            cprintf("index is = %d\n", procIdx);
        }
        panic("NOOOOO");
    }
    // Switch to chosen process.  It is the process's job
    // to release ptable.lock and then reacquire it
    // before jumping back to us.
    c->proc = p;
    switchuvm(p);
    p->state = RUNNING;

    swtch(&(c->scheduler), p->context);
    switchkvm();

    // Process is done running for now.
    // It should have changed its p->state before coming back.
    c->proc = 0;

/*
    for(p=ptable.proc; p<&ptable.proc[NPROC]; p++){
        if(p->state != RUNNABLE)
            continue;
        c->proc=p;
        switchuvm(p);
        p->state = RUNNING;
        swtch(&(c->scheduler), p->context);
        switchkvm();
        c->proc=0;
    }
    */
    release(&ptable.lock);

  }
}

// Enter scheduler.  Must hold only ptable.lock
// and have changed proc->state. Saves and restores
// intena because intena is a property of this
// kernel thread, not this CPU. It should
// be proc->intena and proc->ncli, but that would
// break in the few places where a lock is held but
// there's no process.
void
sched(void)
{
  int intena;
  struct proc *p = myproc();

  if(!holding(&ptable.lock))
    panic("sched ptable.lock");
  if(mycpu()->ncli != 1)
    panic("sched locks");
  if(p->state == RUNNING)
    panic("sched running");
  if(readeflags()&FL_IF)
    panic("sched interruptible");
  intena = mycpu()->intena;
  swtch(&p->context, mycpu()->scheduler);
  mycpu()->intena = intena;
}

// Give up the CPU for one scheduling round.
void
yield(void)
{
  acquire(&ptable.lock);  //DOC: yieldlock
  myproc()->state = RUNNABLE;
  sched();
  release(&ptable.lock);
}

// A fork child's very first scheduling by scheduler()
// will swtch here.  "Return" to user space.
void
forkret(void)
{
  static int first = 1;
  // Still holding ptable.lock from scheduler.
  release(&ptable.lock);

  if (first) {
    // Some initialization functions must be run in the context
    // of a regular process (e.g., they call sleep), and thus cannot
    // be run from main().
    first = 0;
    iinit(ROOTDEV);
    initlog(ROOTDEV);
  }

  // Return to "caller", actually trapret (see allocproc).
}

// Atomically release lock and sleep on chan.
// Reacquires lock when awakened.
void
sleep(void *chan, struct spinlock *lk)
{
  struct proc *p = myproc();
  
  if(p == 0)
    panic("sleep");

  if(lk == 0)
    panic("sleep without lk");

  // Must acquire ptable.lock in order to
  // change p->state and then call sched.
  // Once we hold ptable.lock, we can be
  // guaranteed that we won't miss any wakeup
  // (wakeup runs with ptable.lock locked),
  // so it's okay to release lk.
  if(lk != &ptable.lock){  //DOC: sleeplock0
    acquire(&ptable.lock);  //DOC: sleeplock1
    release(lk);
  }
  // Go to sleep.
  p->chan = chan;
  p->state = SLEEPING;

  sched();

  // Tidy up.
  p->chan = 0;

  // Reacquire original lock.
  if(lk != &ptable.lock){  //DOC: sleeplock2
    release(&ptable.lock);
    acquire(lk);
  }
}

//PAGEBREAK!
// Wake up all processes sleeping on chan.
// The ptable lock must be held.
static void
wakeup1(void *chan)
{
  struct proc *p;

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if(p->state == SLEEPING && p->chan == chan)
      p->state = RUNNABLE;
}

// Wake up all processes sleeping on chan.
void
wakeup(void *chan)
{
  acquire(&ptable.lock);
  wakeup1(chan);
  release(&ptable.lock);
}

// Kill the process with the given pid.
// Process won't exit until it returns
// to user space (see trap in trap.c).
int
kill(int pid)
{
  struct proc *p;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == pid){
      p->killed = 1;
      // Wake process from sleep if necessary.
      if(p->state == SLEEPING)
        p->state = RUNNABLE;
      release(&ptable.lock);
      return 0;
    }
  }
  release(&ptable.lock);
  return -1;
}

//PAGEBREAK: 36
// Print a process listing to console.  For debugging.
// Runs when user types ^P on console.
// No lock to avoid wedging a stuck machine further.
void
procdump(void)
{
  static char *states[] = {
  [UNUSED]    "unused",
  [EMBRYO]    "embryo",
  [SLEEPING]  "sleep ",
  [RUNNABLE]  "runble",
  [RUNNING]   "run   ",
  [ZOMBIE]    "zombie"
  };
  int i;
  struct proc *p;
  char *state;
  uint pc[10];

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state == UNUSED)
      continue;
    if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
      state = states[p->state];
    else
      state = "???";
    cprintf("%d %s %s", p->pid, state, p->name);
    if(p->state == SLEEPING){
      getcallerpcs((uint*)p->context->ebp+2, pc);
      for(i=0; i<10 && pc[i] != 0; i++)
        cprintf(" %p", pc[i]);
    }
    cprintf("\n");
  }
}

//////////////////////////////////proj3. thread
int thread_create(thread_t * thread, void * (*start_routine)(void *), void *arg)
{

  int i, pid, childProcIdx;
  struct proc *np;
  struct proc *curproc = myproc();

  // Allocate process.
  if((np = allocproc()) == 0){
    return -1;
  }
  pid=np->pid;
  childProcIdx=getProcIdx3(pid);
  thread->id=np->pid;
  cprintf(">>idx %d, thread %d function %d\n", childProcIdx, (int)(thread->id), (int)start_routine);
  np->pgdir = curproc->pgdir;
  np->sz = curproc->sz;
  if(curproc->isThread == 1){
    np->parent = curproc->parent;
  }else{
      np->parent = curproc;
  }
  np->isThread=1;
  uint newStack = np->tf->esp;

  *np->tf = *curproc->tf;
  np->tf->eax=0;
  np->tf->eip = (int)(start_routine);
  //np->tf->esp = (int)(ptable.stacks[childProcIdx])+4092;
  //np->tf->esp = (int)(thread->stack)+4092;
  np->tf->esp = newStack;
  *((int*)(np->tf->esp)) = (int)arg;
  *((int*)(np->tf->esp-4))=0xFFFFFFFF;
  np->tf->esp-=4;

  
  // cprintf("%d size\n", sizeof(newStack));
  // Clear %eax so that fork returns 0 in the child.
  np->tf->eax = 0;
  for(i = 0; i < NOFILE; i++)
    if(curproc->ofile[i])
      np->ofile[i] = filedup(curproc->ofile[i]);
  np->cwd = idup(curproc->cwd);
  safestrcpy(np->name, curproc->name, sizeof(curproc->name));

  pid = np->pid;

  acquire(&ptable.lock);

  np->state = RUNNABLE;

  release(&ptable.lock);

  return pid;
  
}

int thread_join(thread_t* thread, void **retval)
{
  struct proc *p;
  int havekids, pid, idx;
  struct proc *curproc = myproc();
  

  acquire(&ptable.lock);
  for(;;){
    // Scan through table looking for exited children.
    havekids = 0;
    for(idx = 0, p = ptable.proc; p < &ptable.proc[NPROC]; p++, idx++){
      if(p->pid != thread->id || p->parent != curproc || p->isThread != 1)
        continue;
      havekids = 1;
      if(p->state == ZOMBIE){
        // Found one.
        pid = p->pid;
        kfree(p->kstack);
        p->kstack = 0;
        p->pid = 0;
        p->parent = 0;
        p->name[0] = 0;
        p->killed = 0;
        p->state = UNUSED;
//      p->isThread = 0;
        *(int*)retval=ptable.retval[idx];
        release(&ptable.lock);
        return pid;
      }
    }

    // No point waiting if we don't have any children.
    if(!havekids || curproc->killed){
      release(&ptable.lock);
      return -1;
    }

    // Wait for children to exit.  (See wakeup1 call in proc_exit.)
    sleep(curproc, &ptable.lock);  //DOC: wait-sleep
  }
}

void thread_exit(void* retval)
{
  struct proc *curproc = myproc();
  struct proc *p;
  int fd, idx;

  if(curproc == initproc)
    panic("init exiting");

  // Close all open files.
  for(fd = 0; fd < NOFILE; fd++){
    if(curproc->ofile[fd]){
      fileclose(curproc->ofile[fd]);
      curproc->ofile[fd] = 0;
    }
  }

  
  begin_op();
  iput(curproc->cwd);
  end_op();
  curproc->cwd = 0;

  acquire(&ptable.lock);
  idx=getProcIdx();
  ptable.retval[idx]=(int)retval;
  cprintf("wtf idx %d, retval %d\n", idx, (int)retval);
  // Parent might be sleeping in wait().
  wakeup1(curproc->parent);


  // Pass abandoned children to init.
  for(idx=0, p = ptable.proc; p < &ptable.proc[NPROC]; idx++, p++){
    if(p->parent == curproc){
      p->parent = initproc;
      if(p->isThread == 1){
          p->kstack = 0;
      }
      if(p->state == ZOMBIE)
        wakeup1(initproc);
    }
  }

  // Jump into the scheduler, never to return.
  curproc->state = ZOMBIE;
  sched();
  panic("zombie exit");
}

