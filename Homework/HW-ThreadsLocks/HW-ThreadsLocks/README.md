# Homework (Simulation)
This program, x86.py, allows you to see how different thread interleavings either cause or avoid race conditions. See the README for details on how the program works and answer the questions below.
```
$ ./x86.py -h
```
```
Usage: x86.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -t NUMTHREADS, --threads=NUMTHREADS
                        number of threads
  -p PROGFILE, --program=PROGFILE
                        source program (in .s)
  -i INTFREQ, --interrupt=INTFREQ
                        interrupt frequency
  -P PROCSCHED, --procsched=PROCSCHED
                        control exactly which thread runs when
  -r, --randints        if interrupts are random
  -a ARGV, --argv=ARGV  comma-separated per-thread args (e.g., ax=1,ax=2 sets
                        thread 0 ax reg to 1 and thread 1 ax reg to 2);
                        specify multiple regs per thread via colon-separated
                        list (e.g., ax=1:bx=2,cx=3 sets thread 0 ax and bx and
                        just cx for thread 1)
  -L LOADADDR, --loadaddr=LOADADDR
                        address where to load code
  -m MEMSIZE, --memsize=MEMSIZE
                        size of address space (KB)
  -M MEMTRACE, --memtrace=MEMTRACE
                        comma-separated list of addrs to trace (e.g.,
                        20000,20001)
  -R REGTRACE, --regtrace=REGTRACE
                        comma-separated list of regs to trace (e.g.,
                        ax,bx,cx,dx)
  -C, --cctrace         should we trace condition codes
  -S, --printstats      print some extra stats
  -v, --verbose         print some extra info
  -H HEADERCOUNT, --headercount=HEADERCOUNT
                        how often to print a row header
  -c, --compute         compute answers for me
```
## Questions
1. Examine flag.s. This code “implements” locking with a single memory flag. Can you understand the assembly?
```
$ ./x86.py -p flag.s -M flag -R ax,bx -C -c
```
flag => Shared memory address, enter the critical section when it is 0
count => increase count by one store in ax
sub => rest by one register bx then test 

2. When you run with the defaults, does flag.s work? Use the -M and -R flags to trace variables and registers (and turn on -c to see their values). Can you predict what value will end up in flag?
```
$ ./x86.py -p flag.s -M flag,count -R ax,bx -C -c
```
In the first threat values end up:
flag  = 0
ax = 0
bx = 0

second thread values end up:
flag  = 0
ax = 2
bx = -1

ax count is equal 2 because count increase by 1.

3. Change the value of the register %bx with the -a flag (e.g., -a bx=2,bx=2 if you are running just two threads). What does the code do? How does it change your answer for the question above?
```
$ ./x86.py -p flag.s -M flag,count -R ax,bx -C -a bx=2,bx=2 -c
```
In the first threat values end up:
flag  = 0
ax = 0
bx = 0

second thread values end up:
flag  = 0
ax = 2
bx = -1

Each threat run twice because bx=2 but the end values doesn't change.

4. Set bx to a high value for each thread, and then use the -i flag to generate different interrupt frequencies; what values lead to a bad outcomes? Which lead to good outcomes?

bad outcomes
```
$ ./x86.py -p flag.s -M flag,count -R ax,bx -C -a bx=10,bx=10 -i 1-10,12,13,14,17 -c
```
good outcomes
```
$ ./x86.py -p flag.s -M flag,count -R ax,bx -C -a bx=10,bx=10 -i 11,15,16 -c
```
5. Now let’s look at the program test-and-set.s. First, try to understand the code, which uses the xchg instruction to build a simple locking primitive. 
```
$ ./x86.py -p test-and-set.s
```
```
ARG seed 0
ARG numthreads 2
ARG program test-and-set.s
ARG interrupt frequency 50
ARG interrupt randomness False
ARG procsched 
ARG argv 
ARG load address 1000
ARG memsize 128
ARG memtrace 
ARG regtrace 
ARG cctrace False
ARG printstats False
ARG verbose False

1000 mov  $1, %ax
1001 xchg %ax, mutex
1002 test $0, %ax
1003 jne  .acquire
1004 mov  count, %ax
1005 add  $1, %ax
1006 mov  %ax, count
1007 mov  $0, mutex
1008 sub  $1, %bx
1009 test $0, %bx
1010 jgt .top
1011 halt

```
How is the lock acquire written?
How about lock release?
Spin Lock Using Test-and-set
```
typedef struct __lock_t {
2 int flag;
3 } lock_t;
4
5 void init(lock_t *lock) {
6 // 0: lock is available, 1: lock is held
7 lock->flag = 0;
8 }
9
10 void lock(lock_t *lock) {
11 while (TestAndSet(&lock->flag, 1) == 1)
12 ; // spin-wait (do nothing)
13 }
14
15 void unlock(lock_t *lock) {
16 lock->flag = 0;
17 }
```

6. Now run the code, changing the value of the interrupt interval (-i) again, and making sure to loop for a number of times. Does the code always work as expected? Does it sometimes lead to an inefficient use of the CPU? How could you quantify that?

```
$ ./x86.py -p test-and-set.s -i 40
```
```
$ ./x86.py -p test-and-set.s -i 60
```
It always work as expected.
the higher the CPU usage, the lower the efficiency

7. Use the -P flag to generate specific tests of the locking code. For example, run a schedule that grabs the lock in the first thread, but then tries to acquire it in the second. Does the right thing happen? What else should you test?
```
$ ./x86.py -p test-and-set.s -M count,mutex -R ax,bx -P 0011 -c
```
The two threads have the same starting point and the same instructions executed, so that mean that the critical section must be in the same line .

8. Now let’s look at the code in peterson.s, which implements Peterson’s algorithm (mentioned in a sidebar in the text). Study the code and see if you can make sense of it.
```
$ ./x86.py -p peterson.s
```

9. Now run the code with different values of -i. What kinds of different behavior do you see? Make sure to set the thread IDs appropriately (using -a bx=0,bx=1 for example) as the code assumes it.
```
$ ./x86.py -p peterson.s -M count,flag,turn -R ax,cx -a bx=0,bx=1 -c -i 2
```
```
$ ./x86.py -p peterson.s -M count,flag,turn -R ax,cx -a bx=0,bx=1 -c -i 10
```
```
$ ./x86.py -p peterson.s -M count,flag,turn -R ax,cx -a bx=0,bx=1 -c -i 3
```

The count setting of the critical section has only one active thread, and the count can always be correctly updated to 2


10. Can you control the scheduling (with the -P flag) to “prove” that the code works? What are the different cases you should show hold? Think about mutual exclusion and deadlock avoidance.
```
$ ./x86.py -p peterson.s -M count,flag,turn -R ax,cx -a bx=0,bx=1 -c -P 1111
```
Does not include all processes fail.
```
$ ./x86.py -p peterson.s -M count,flag,turn -R ax,cx -a bx=0,bx=1 -c -P 0111
```
it returns successfully
```
$ ./x86.py -p peterson.s -M count,flag,turn -R ax,cx -a bx=0,bx=1 -c -P 0100
```
it returns successfully
```
$ ./x86.py -p peterson.s -M count,flag,turn -R ax,cx -a bx=0,bx=1 -c -P 0110
```
it returns successfully
```
$ ./x86.py -p peterson.s -M count,flag,turn -R ax,cx -a bx=0,bx=1 -c -P 0000
```
Does not include all processes fail.
A single lock will not deadlock. Two sets of locks can cause a deadlock.

11. Now study the code for the ticket lock in ticket.s. Does it match the code in the chapter? Then run with the following flags: -a bx=1000,bx=1000 (causing each thread to loop through the critical section 1000 times). Watch what happens; do the threads spend much time spin-waiting for the lock?
```
$ ./x86.py -p ticket.s -M count,ticket,turn -R ax,bx,cx -a bx=1000,bx=1000 -c
```
12. How does the code behave as you add more threads?
```
$ ./x86.py -p ticket.s -t 3 -M count,ticket,turn -R ax,bx -c
```
```
$ ./x86.py -p ticket.s -t 5 -M count,ticket,turn -R ax,bx -c
```
```
$ ./x86.py -p ticket.s -t 10 -M count,ticket,turn -R ax,bx -c
```
Because of the fairness of tickets, there will be situations where two consecutive time slices are idling. t0 is executing the critical section, t1 comes in, ticket+1, and then waits in its own time slice; t0 comes out of the critical section, turn+1, then ticket+1, found ticket> turn in this time slice, so the other Wait then can be executed when switching to t1

13. Now examine yield.s, in which a yield instruction enables one thread to yield control of the CPU (realistically, this would be an OS primitive, but for the simplicity, we assume an instruction does the task). Find a scenario where test-and-set.s wastes cycles spinning, but yield.s does not. How many instructions are saved? In what scenarios do these savings arise?
```
$ ./x86.py -p yield.s -M count,mutex -R ax,bx -a bx=5,bx=5 -c -i 7
```
```
$ ./x86.py -p test-and-set.s -M count,mutex -R ax,bx -a bx=5,bx=5 -c -i 7
```

14. Finally, examine test-and-test-and-set.s. What does this lock do? What kind of savings does it introduce as compared to test-and-set.s?
```
$ ./x86.py -p test-and-test-and-set.s.
```
Change mutex to 1 only if lock is free. That will avoid unnecessary writing.
