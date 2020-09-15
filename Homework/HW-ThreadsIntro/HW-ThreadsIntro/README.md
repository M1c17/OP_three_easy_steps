# Homework (Simulation)
This program, x86.py, allows you to see how different thread interleavings either cause or avoid race conditions. See the README for details on how the program works, then answer the questions below.

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
  -c, --compute         compute answers for me
```
## Questions
1. Let’s examine a simple program, “loop.s”. First, just read and understand it. Then, run it with these arguments (./x86.py -p loop.s -t 1 -i 100 -R dx) This specifies a single thread, an interrupt every 100 instructions, and tracing of register %dx. What will %dx be during the run? Use the -c flag to check your answers; the answers, on the left, show the value of the register (or memory value) after the instruction on the right has run.
```
$ ./x86.py -p loop.s -t 1 -i 100 -R dx -c
```
```
dx  Thread 0        
0
-1  1000 sub  $1,%dx
-1  1001 test $0,%dx
-1  1002 jgte .top
-1  1003 halt
```

2. Same code, different flags: (./x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx) This specifies two threads, and initializes each %dx to 3. What values will %dx see? Run with -c to check. Does the presence of multiple threads affect your calculations? Is there a race in this code?
```
$ ./x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx -c
```
```
ARG seed 0
ARG numthreads 2
ARG program loop.s
ARG interrupt frequency 100
ARG interrupt randomness False
ARG argv dx=3,dx=3
ARG load address 1000
ARG memsize 128
ARG memtrace 
ARG regtrace dx
ARG cctrace False
ARG printstats False
ARG verbose False
```
two threads, and initializes each %dx to 3 => No competition, No shared data

3. Run this: ./x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx This makes the interrupt interval small/random; use different seeds (-s) to see different interleavings. Does the interrupt frequency change anything?

```
$ ./x86.py -p loop.s -t 2 -i 3 -r -a dx=3,dx=3 -R dx -c 
```
The interrumpt frequency is random since we set the command -r -> for interrupt interval small/random. It doesn't matter if the context switch is more frequent, because there is still no shared memory.

4. Now, a different program, looping-race-nolock.s, which accesses a shared variable located at address 2000; we’ll call this variable value. Run it with a single thread to confirm your understanding: ./x86.py -p looping-race-nolock.s -t 1 -M 2000 What is value (i.e., at memory address 2000) throughout the run? Use -c to check.
```
$ ./x86.py -p looping-race-nolock.s -t 1 -M 2000 -c
2000          Thread 0         
0   
0   1000 mov 2000, %ax
0   1001 add $1, %ax
1   1002 mov %ax, 2000
1   1003 sub  $1, %bx
1   1004 test $0, %bx
1   1005 jgt .top
1   1006 halt
```
Single thread loop once. Trace M[2000] = 1

5. Run with multiple iterations/threads: ./x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000 Why does each thread loop three times? What is final value of value?
```
$ ./x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000 -c
```
Because loop 3 times bx = 3 adding value to register ax store in register 2000.
The two threads are not interleaved.
Final value stored in M[2000] = 6.

6. Run with random interrupt intervals: ./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0 with different seeds (-s 1, -s 2, etc.) Can you tell by looking at the thread interleaving what the final value of value will be? Does the timing of the interrupt matter? Where can it safely occur? Where not? In other words, where is the critical section exactly?

```
$ ./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0 -c
```
```
$ ./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 1 -c
```
```
$ ./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 2 -c
```
the value stored in M[2000] = 1 or 2 depends on if the interrupts happen after or before stored the value in M[2000]. That mean the timing of the interrupt matter.
The critical section is after and before 1002 mov %ax, 2000.

7. Now examine fixed interrupt intervals: ./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1  What about when you change -i 2, -i 3, etc.? 
```
$ ./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1 -c
```
```
$ ./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 2 -c
```
```
$ ./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 3 -c
```
What will the final value of the shared variable value be?
-i 1 => 1
-i 2 => 1
-i 3 => 2
For which interrupt intervals does the program give the “correct” answer?
-i >= 3

8. Run the same for more loops (e.g., set -a bx=100).  Which intervals are surprising?
```
$ ./x86.py -p looping-race-nolock.s -a bx=100 -t 2 -M 2000 -i 1 -c
```
```
$ ./x86.py -p looping-race-nolock.s -a bx=100 -t 2 -M 2000 -i 2 -c
```
```
$ ./x86.py -p looping-race-nolock.s -a bx=100 -t 2 -M 2000 -i 3 -c
```
What interrupt intervals (-i) lead to a correct outcome?
-i >= 597 the Final value looping bx=100 is 200
Which intervals are surprising?
with -i = 3 * x (x = 1, 2, 3,...) Final result looping bx=100 is < 150

9. One last program: wait-for-me.s. Run: ./x86.py -p wait-for-me.s -a ax=1,ax=0 -R ax -M 2000 This sets the %ax register to 1 for thread 0, and 0 for thread 1, and watches %ax and memory location 2000. How should the code behave? How is the value at location 2000 being used by the threads? What will its final value be?
```
$ ./x86.py -p wait-for-me.s -a ax=1,ax=0 -R ax -M 2000 -c
```
Thread 0 set M[2000] = 1 after that exit.
Thread 1 ax = 0 store the value M[2000] in register cx compare jne .waiter (# jump if test'd values are not equal to) to waiter loop and wait for M[2000]=1 to exit.

10. Now switch the inputs: ./x86.py -p wait-for-me.s -a ax=0,ax=1 -R ax -M 2000 How do the threads behave? What is thread 0 doing? How would changing the interrupt interval (e.g., -i 1000, or perhaps to use random intervals) change the trace outcome? 
```
$ ./x86.py -p wait-for-me.s -a ax=0,ax=1 -R ax -M 2000 -c
```
In this case thread 0 -> ax = 0 never set M[2000] to 1 and exit after interrupt frequency 50 then thread 1 set M[2000] to 1 then switch to thread 0 compare that M[2000] is 1 and exit.

Is the program efficiently using the CPU?
No actually the first case was more efficiently.
