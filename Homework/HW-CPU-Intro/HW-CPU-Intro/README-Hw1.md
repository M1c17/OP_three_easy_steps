# THE ABSTRACTION: THE PROCESS
## Homework (Simulation)

This program, process-run.py, allows you to see how process states
change as programs run and either use the CPU (e.g., perform an add
instruction) or do I/O (e.g., send a request to a disk and wait for it to
complete). See the README for details.

## Questions

1. Run process-run.py with the following flags: -l 5:100,5:100.
What should the CPU utilization be (e.g., the percent of time the CPU is in use?) Why do you know this? Use the -c and -p flags to see if you were right.

To run the program and get its options:

`prompt> python process-run.py -h`

```
Usage: process-run.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -l PROCESS_LIST, --processlist=PROCESS_LIST
                        a comma-separated list of processes to run, in the
                        form X1:Y1,X2:Y2,... where X is the number of
                        instructions that process should run, and Y the
                        chances (from 0 to 100) that an instruction will use
                        the CPU or issue an IO
  -L IO_LENGTH, --iolength=IO_LENGTH
                        how long an IO takes
  -S PROCESS_SWITCH_BEHAVIOR, --switch=PROCESS_SWITCH_BEHAVIOR
                        when to switch between processes: SWITCH_ON_IO,
                        SWITCH_ON_END
  -I IO_DONE_BEHAVIOR, --iodone=IO_DONE_BEHAVIOR
                        type of behavior when IO ends: IO_RUN_LATER,
                        IO_RUN_IMMEDIATE
  -c                    compute answers for me
  -p, --printstats      print statistics at end; only useful with -c flag
                        (otherwise stats are not printed)
```
the most important option to understand is the PROCESS_LIST (as specified by
the -l or --processlist flags) which specifies exactly what each running
program (or "process") will do.

`prompt> ./process-run.py -l  5:100`

a simple run that just has one program being run, and that program only
uses the CPU (it does no IO).

```
Produce a trace of what would happen when you run these processes:
Process 0
  cpu
  cpu
  cpu
  cpu
  cpu

Important behaviors:
  System will switch when the current process is FINISHED or ISSUES AN IO
  After IOs, the process issuing the IO will run LATER (when it is its turn)
```
Here, the process we specified is "5:100" which means it should consist of 5 instructions, and the chances that each instruction is a CPU instruction are
100%.
You can see what happen to the process with the flag -c

`prompt> ./process-run.py -l 5:100 -c`

```
Time     PID: 0        CPU        IOs 
1     RUN:cpu          1            
2     RUN:cpu          1            
3     RUN:cpu          1            
4     RUN:cpu          1            
5     RUN:cpu          1  

```
You can see the process is in the RUN state and the finishes, using the CPU the whole time and not doing any I/Os.

Now, let's running two process at the same time to see what happen:

`prompt> ./process-run.py -l 5:100,5:100`

```
Produce a trace of what would happen when you run these processes:
Process 0
  cpu
  cpu
  cpu
  cpu
  cpu

Process 1
  cpu
  cpu
  cpu
  cpu
  cpu

Important behaviors:
  System will switch when the current process is FINISHED or ISSUES AN IO
  After IOs, the process issuing the IO will run LATER (when it is its turn)
```
In this case, two different processes run, each again just using the CPU. What happens when the operating system runs them? Let's find out:

`prompt> ./process-run.py -l 5:100,5:100 -c`

```
Time     PID: 0     PID: 1        CPU        IOs 
 1     RUN:cpu      READY          1            
 2     RUN:cpu      READY          1            
 3     RUN:cpu      READY          1            
 4     RUN:cpu      READY          1            
 5     RUN:cpu      READY          1            
 6        DONE    RUN:cpu          1            
 7        DONE    RUN:cpu          1            
 8        DONE    RUN:cpu          1            
 9        DONE    RUN:cpu          1            
10        DONE    RUN:cpu          1 

```
As you can see above, the first process PDI: 0 runs, while PDI: 1 is ready to run but just waits until PDI: 0 is done.
When PDI:0 is finishes, it moves to the DONE state, while PDI:1 runs. When 1 finishes, the trace is done.

2. Now run with these flags: ./process-run.py -l 4:100,1:0.
These flags specify one process with 4 instructions (all to use the CPU), and one that simply issues an I/O and waits for it to be done.
How long does it take to complete both processes? Use -c and -p to find out if you were right.

`./process-run.py -l 4:100,1:0`

```
Produce a trace of what would happen when you run these processes:
Process 0
  cpu
  cpu
  cpu
  cpu

Process 1
  io

Important behaviors:
  System will switch when the current process is FINISHED or ISSUES AN IO
  After IOs, the process issuing the IO will run LATER (when it is its turn)
```
Here, there are two different processes one running the CPU and the other one issues an I/O.
Now let's find out when the OS run this process.

`./process-run.py -l 4:100,1:0 -c`

```
Time     PID: 0     PID: 1        CPU        IOs 
 1     RUN:cpu      READY          1            
 2     RUN:cpu      READY          1            
 3     RUN:cpu      READY          1            
 4     RUN:cpu      READY          1            
 5        DONE     RUN:io          1            
 6        DONE    WAITING                     1 
 7        DONE    WAITING                     1 
 8        DONE    WAITING                     1 
 9        DONE    WAITING                     1 
10*       DONE       DONE 
```

As we can see above PID 0 run first running CPU and PID 1 is READY to run then when PID 0 is DONE, PID 1 RUN:io then the process is WAITING on I/O after this state on Time 10 both PID 0 and PID 1 are in state DONE.


`./process-run.py -l 4:100,1:0 -c`

running the flag -p we can know exactly the Total time it take both PID.

`./process-run.py -l 4:100,1:0 -c -p`

```
Time     PID: 0     PID: 1        CPU        IOs 
  1     RUN:cpu      READY          1            
  2     RUN:cpu      READY          1            
  3     RUN:cpu      READY          1            
  4     RUN:cpu      READY          1            
  5        DONE     RUN:io          1            
  6        DONE    WAITING                     1 
  7        DONE    WAITING                     1 
  8        DONE    WAITING                     1 
  9        DONE    WAITING                     1 
 10*       DONE       DONE                       

Stats: Total Time 10
Stats: CPU Busy 5 (50.00%)
Stats: IO Busy  4 (40.00%)
```
CPU 50% Busy and IO 40% Busy takes place  on the OS.

3. Switch the order of the processes: -l 1:0,4:100. What happens now? Does switching the order matter? Why? (As always, use -c and -p to see if you were right)

Let's figure out !!!

`./process-run.py -l 1:0,4:100 -c -p`

```
Time     PID: 0     PID: 1        CPU        IOs 
  1      RUN:io      READY          1            
  2     WAITING    RUN:cpu          1          1 
  3     WAITING    RUN:cpu          1          1 
  4     WAITING    RUN:cpu          1          1 
  5     WAITING    RUN:cpu          1          1 
  6*       DONE       DONE                       

Stats: Total Time 6
Stats: CPU Busy 5 (83.33%)
Stats: IO Busy  4 (66.67%)
```
As we can see switching the order does its matter.
the PID is RUN:io first then WAITING and at the same time the process PDI 0 is in that state the process PID1 can RUN: cpu making the Total time => 6 then both process are DONE at same time. It will improve the CPU utilization.

4. We’ll now explore some of the other flags. One important flag is -S, which determines how the system reacts when a process issues an I/O. With the flag set to SWITCH ON END, the system will NOT switch to another process while one is doing I/O, instead waiting until the process is completely finished. What happens when you run the following two processes (-l 1:0,4:100 -c -S SWITCH ON END), one doing I/O and the other doing CPU work?

`./process-run.py -l 1:0,4:100 -c -p -S SWITCH_ON_END`

-S -> determines how the system reacts when a process issues an I/O

```
Time     PID: 0     PID: 1        CPU        IOs 
  1      RUN:io      READY          1            
  2     WAITING      READY                     1 
  3     WAITING      READY                     1 
  4     WAITING      READY                     1 
  5     WAITING      READY                     1 
  6*       DONE    RUN:cpu          1            
  7        DONE    RUN:cpu          1            
  8        DONE    RUN:cpu          1            
  9        DONE    RUN:cpu          1            

Stats: Total Time 9
Stats: CPU Busy 5 (55.56%)
Stats: IO Busy  4 (44.44%) 
```
In this case we can see the process PID 0 first RUN:io and change to the state WAITING since we  set the flag that set to SWITCH ON END, the system will NOT switch to another process while one is doing I/O, instead waiting until the process is completely finished after the that the PID RUN:cpu state.

5. Now, run the same processes, but with the switching behavior set to switch to another process whenever one is WAITING for I/O (-l 1:0,4:100 -c -S SWITCH ON IO). What happens now? Use -c and -p to confirm that you are right.

`./process-run.py -l 1:0,4:100 -c -p -S SWITCH_ON_IO`

```
Time     PID: 0     PID: 1        CPU        IOs 
  1      RUN:io      READY          1            
  2     WAITING    RUN:cpu          1          1 
  3     WAITING    RUN:cpu          1          1 
  4     WAITING    RUN:cpu          1          1 
  5     WAITING    RUN:cpu          1          1 
  6*       DONE       DONE                       

Stats: Total Time 6
Stats: CPU Busy 5 (83.33%)
Stats: IO Busy  4 (66.67%)
```
In this case we are having the same bahaviour as the question 3 since we set the flag SWITCH_ON_IO the PID 1 can RUN:cpu while PID 0 is WAITING so in this case is improving CPU utilization.

6. One other important behavior is what to do when an I/O completes. With -I IO RUN LATER, when an I/O completes, the process that issued it is not necessarily run right away; rather, whatever was running at the time keeps running. What happens when you run this combination of processes? (Run ./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH ON IO -I IO RUN LATER -c -p) Are system resources being effectively utilized?

`./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_LATER -c -p`

```
Time     PID: 0     PID: 1     PID: 2     PID: 3        CPU        IOs 
  1      RUN:io      READY      READY      READY          1            
  2     WAITING    RUN:cpu      READY      READY          1          1 
  3     WAITING    RUN:cpu      READY      READY          1          1 
  4     WAITING    RUN:cpu      READY      READY          1          1 
  5     WAITING    RUN:cpu      READY      READY          1          1 
  6*      READY    RUN:cpu      READY      READY          1            
  7       READY       DONE    RUN:cpu      READY          1            
  8       READY       DONE    RUN:cpu      READY          1            
  9       READY       DONE    RUN:cpu      READY          1            
 10       READY       DONE    RUN:cpu      READY          1            
 11       READY       DONE    RUN:cpu      READY          1            
 12       READY       DONE       DONE    RUN:cpu          1            
 13       READY       DONE       DONE    RUN:cpu          1            
 14       READY       DONE       DONE    RUN:cpu          1            
 15       READY       DONE       DONE    RUN:cpu          1            
 16       READY       DONE       DONE    RUN:cpu          1            
 17      RUN:io       DONE       DONE       DONE          1            
 18     WAITING       DONE       DONE       DONE                     1 
 19     WAITING       DONE       DONE       DONE                     1 
 20     WAITING       DONE       DONE       DONE                     1 
 21     WAITING       DONE       DONE       DONE                     1 
 22*     RUN:io       DONE       DONE       DONE          1            
 23     WAITING       DONE       DONE       DONE                     1 
 24     WAITING       DONE       DONE       DONE                     1 
 25     WAITING       DONE       DONE       DONE                     1 
 26     WAITING       DONE       DONE       DONE                     1 
 27*       DONE       DONE       DONE       DONE                       

Stats: Total Time 27
Stats: CPU Busy 18 (66.67%)
Stats: IO Busy  12 (44.44%)
```

Here we have 3 prcesses. PID 0 RUN:io while is WAITING PID1 RUN:cpu when PID 1 DONE PID2 RUN:cpu while PID 0 and PID 3 sets state READY when PID 2 state DONE PID3 RUN:cpu then in Time 17 when PID 3 DONE -> PID 0 RUN:io set state WAITING until Time 27 when all the PID's are state DONE.
This behavior is it posible since we set IO RUN LATER (-I IO RUN LATER, when an I/O completes, the process that issued it is not necessarily run right away; rather, whatever was running at the time keeps running) PID 0 state READY when others process done to runs  and finally IO runs the ramaining.

Are system resources being effectively utilized?

Not effective enought. When the first IO request finished, the issued process will not continue to next IO request. It will stay ready until the other three CPU work processes completed.

7. Now run the same processes, but with -I IO RUN IMMEDIATE set,
which immediately runs the process that issued the I/O. How does
this behavior differ? Why might running a process that just completed an I/O again be a good idea?

`./process-run.py -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_IMMEDIATE -c -p`

```
Time     PID: 0     PID: 1     PID: 2     PID: 3        CPU        IOs 
  1      RUN:io      READY      READY      READY          1            
  2     WAITING    RUN:cpu      READY      READY          1          1 
  3     WAITING    RUN:cpu      READY      READY          1          1 
  4     WAITING    RUN:cpu      READY      READY          1          1 
  5     WAITING    RUN:cpu      READY      READY          1          1 
  6*     RUN:io      READY      READY      READY          1            
  7     WAITING    RUN:cpu      READY      READY          1          1 
  8     WAITING       DONE    RUN:cpu      READY          1          1 
  9     WAITING       DONE    RUN:cpu      READY          1          1 
 10     WAITING       DONE    RUN:cpu      READY          1          1 
 11*     RUN:io       DONE      READY      READY          1            
 12     WAITING       DONE    RUN:cpu      READY          1          1 
 13     WAITING       DONE    RUN:cpu      READY          1          1 
 14     WAITING       DONE       DONE    RUN:cpu          1          1 
 15     WAITING       DONE       DONE    RUN:cpu          1          1 
 16*       DONE       DONE       DONE    RUN:cpu          1            
 17        DONE       DONE       DONE    RUN:cpu          1            
 18        DONE       DONE       DONE    RUN:cpu          1            

Stats: Total Time 18
Stats: CPU Busy 18 (100.00%)
Stats: IO Busy  12 (66.67%)
```
How does this behavior differ?
Unlike the exercise 6 here PID 0 RUN:io while the others process are in state READY or DONE and RUN:cpu while PID 0 is in state WAITING or DONE.

Why might running a process that just completed an I/O again be a good idea?
When the IO request completed, the IO process will immediately take the CPU usage. In this case, it will reduce time cost and improve CPU utilization.

8. Now run with some randomly generated processes: -s 1 -l 3:50,3:50
or -s 2 -l 3:50,3:50 or -s 3 -l 3:50,3:50. See if you can predict how the trace will turn out. What happens when you use the flag -I IO RUN IMMEDIATE vs. -I IO RUN LATER? What happens when you use -S SWITCH ON IO vs. -S SWITCH ON END?

## Case1 - IO_RUN_IMMEDIATE VS. IO_RUN_LATER:
`./process-run.py -s 1 -l 3:50,3:50 -I IO_RUN_LATER -c -p`

```
Time     PID: 0     PID: 1        CPU        IOs 
  1     RUN:cpu      READY          1            
  2      RUN:io      READY          1            
  3     WAITING    RUN:cpu          1          1 
  4     WAITING    RUN:cpu          1          1 
  5     WAITING    RUN:cpu          1          1 
  6     WAITING       DONE                     1 
  7*     RUN:io       DONE          1            
  8     WAITING       DONE                     1 
  9     WAITING       DONE                     1 
 10     WAITING       DONE                     1 
 11     WAITING       DONE                     1 
 12*       DONE       DONE                       

Stats: Total Time 12
Stats: CPU Busy 6 (50.00%)
Stats: IO Busy  8 (66.67%)
```
`./process-run.py -s 1 -l 3:50,3:50 -I IO_RUN_IMMEDIATE -c -p`

```
Time     PID: 0     PID: 1        CPU        IOs 
  1     RUN:cpu      READY          1            
  2      RUN:io      READY          1            
  3     WAITING    RUN:cpu          1          1 
  4     WAITING    RUN:cpu          1          1 
  5     WAITING    RUN:cpu          1          1 
  6     WAITING       DONE                     1 
  7*     RUN:io       DONE          1            
  8     WAITING       DONE                     1 
  9     WAITING       DONE                     1 
 10     WAITING       DONE                     1 
 11     WAITING       DONE                     1 
 12*       DONE       DONE                       

Stats: Total Time 12
Stats: CPU Busy 6 (50.00%)
Stats: IO Busy  8 (66.67%)
```
Same results between IO_RUN_IMMEDIATE and IO_RUN_LATER because IO request completed after CPU request completed.

## Case2 SWITCH_ON_IO VS. SWITCH_ON_END:
`./process-run.py -s 1 -l 3:50,3:50 -c -p -S SWITCH_ON_END`

```
Time     PID: 0     PID: 1        CPU        IOs 
  1     RUN:cpu      READY          1            
  2      RUN:io      READY          1            
  3     WAITING      READY                     1 
  4     WAITING      READY                     1 
  5     WAITING      READY                     1 
  6     WAITING      READY                     1 
  7*     RUN:io      READY          1            
  8     WAITING      READY                     1 
  9     WAITING      READY                     1 
 10     WAITING      READY                     1 
 11     WAITING      READY                     1 
 12*       DONE    RUN:cpu          1            
 13        DONE    RUN:cpu          1            
 14        DONE    RUN:cpu          1            

Stats: Total Time 14
Stats: CPU Busy 6 (42.86%)
Stats: IO Busy  8 (57.14%)
```
`./process-run.py -s 1 -l 3:50,3:50 -c -p -S SWITCH_ON_IO`

```
Time     PID: 0     PID: 1        CPU        IOs 
  1     RUN:cpu      READY          1            
  2      RUN:io      READY          1            
  3     WAITING    RUN:cpu          1          1 
  4     WAITING    RUN:cpu          1          1 
  5     WAITING    RUN:cpu          1          1 
  6     WAITING       DONE                     1 
  7*     RUN:io       DONE          1            
  8     WAITING       DONE                     1 
  9     WAITING       DONE                     1 
 10     WAITING       DONE                     1 
 11     WAITING       DONE                     1 
 12*       DONE       DONE                       

Stats: Total Time 12
Stats: CPU Busy 6 (50.00%)
Stats: IO Busy  8 (66.67%)
```
Different behavior: SWITCH_ON_END, the CPU process will run after IO process completed and cause longer time cost. SWITCH_ON_IO, CPU will run while IO state WAITING and IO will finish run and finish process after CPU is DONE.

## Case1 - IO_RUN_IMMEDIATE VS. IO_RUN_LATER:
`./process-run.py -s 2 -l 3:50,3:50 -I IO_RUN_LATER -c -p`

```
Time     PID: 0     PID: 1        CPU        IOs 
  1      RUN:io      READY          1            
  2     WAITING    RUN:cpu          1          1 
  3     WAITING     RUN:io          1          1 
  4     WAITING    WAITING                     2 
  5     WAITING    WAITING                     2 
  6*     RUN:io    WAITING          1          1 
  7     WAITING    WAITING                     2 
  8*    WAITING     RUN:io          1          1 
  9     WAITING    WAITING                     2 
 10     WAITING    WAITING                     2 
 11*    RUN:cpu    WAITING          1          1 
 12        DONE    WAITING                     1 
 13*       DONE       DONE                       

Stats: Total Time 13
Stats: CPU Busy 6 (46.15%)
Stats: IO Busy  11 (84.62%)
```
`./process-run.py -s 2 -l 3:50,3:50 -I IO_RUN_IMMEDIATE -c -p`

```
Time     PID: 0     PID: 1        CPU        IOs 
  1      RUN:io      READY          1            
  2     WAITING    RUN:cpu          1          1 
  3     WAITING     RUN:io          1          1 
  4     WAITING    WAITING                     2 
  5     WAITING    WAITING                     2 
  6*     RUN:io    WAITING          1          1 
  7     WAITING    WAITING                     2 
  8*    WAITING     RUN:io          1          1 
  9     WAITING    WAITING                     2 
 10     WAITING    WAITING                     2 
 11*    RUN:cpu    WAITING          1          1 
 12        DONE    WAITING                     1 
 13*       DONE       DONE                       

Stats: Total Time 13
Stats: CPU Busy 6 (46.15%)
Stats: IO Busy  11 (84.62%)
```
## Case2 SWITCH_ON_IO VS. SWITCH_ON_END:
`./process-run.py -s 2 -l 3:50,3:50 -c -p -S SWITCH_ON_END`

```
Time     PID: 0     PID: 1        CPU        IOs 
  1      RUN:io      READY          1            
  2     WAITING      READY                     1 
  3     WAITING      READY                     1 
  4     WAITING      READY                     1 
  5     WAITING      READY                     1 
  6*     RUN:io      READY          1            
  7     WAITING      READY                     1 
  8     WAITING      READY                     1 
  9     WAITING      READY                     1 
 10     WAITING      READY                     1 
 11*    RUN:cpu      READY          1            
 12        DONE    RUN:cpu          1            
 13        DONE     RUN:io          1            
 14        DONE    WAITING                     1 
 15        DONE    WAITING                     1 
 16        DONE    WAITING                     1 
 17        DONE    WAITING                     1 
 18*       DONE     RUN:io          1            
 19        DONE    WAITING                     1 
 20        DONE    WAITING                     1 
 21        DONE    WAITING                     1 
 22        DONE    WAITING                     1 
 23*       DONE       DONE                       

Stats: Total Time 23
Stats: CPU Busy 6 (26.09%)
Stats: IO Busy  16 (69.57%)
```
`./process-run.py -s 2 -l 3:50,3:50 -c -p -S SWITCH_ON_IO`

```
Time     PID: 0     PID: 1        CPU        IOs 
  1      RUN:io      READY          1            
  2     WAITING    RUN:cpu          1          1 
  3     WAITING     RUN:io          1          1 
  4     WAITING    WAITING                     2 
  5     WAITING    WAITING                     2 
  6*     RUN:io    WAITING          1          1 
  7     WAITING    WAITING                     2 
  8*    WAITING     RUN:io          1          1 
  9     WAITING    WAITING                     2 
 10     WAITING    WAITING                     2 
 11*    RUN:cpu    WAITING          1          1 
 12        DONE    WAITING                     1 
 13*       DONE       DONE                       

Stats: Total Time 13
Stats: CPU Busy 6 (46.15%)
Stats: IO Busy  11 (84.62%)
```
## Case1 - IO_RUN_IMMEDIATE VS. IO_RUN_LATER:
`./process-run.py -s 3 -l 3:50,3:50 -I IO_RUN_LATER -c -p`

```
Time     PID: 0     PID: 1        CPU        IOs 
  1     RUN:cpu      READY          1            
  2      RUN:io      READY          1            
  3     WAITING     RUN:io          1          1 
  4     WAITING    WAITING                     2 
  5     WAITING    WAITING                     2 
  6     WAITING    WAITING                     2 
  7*    RUN:cpu    WAITING          1          1 
  8*       DONE     RUN:io          1            
  9        DONE    WAITING                     1 
 10        DONE    WAITING                     1 
 11        DONE    WAITING                     1 
 12        DONE    WAITING                     1 
 13*       DONE    RUN:cpu          1            

Stats: Total Time 13
Stats: CPU Busy 6 (46.15%)
Stats: IO Busy  9 (69.23%)
```

`./process-run.py -s 3 -l 3:50,3:50 -I IO_RUN_IMMEDIATE -c -p`

```
Time     PID: 0     PID: 1        CPU        IOs 
  1     RUN:cpu      READY          1            
  2      RUN:io      READY          1            
  3     WAITING     RUN:io          1          1 
  4     WAITING    WAITING                     2 
  5     WAITING    WAITING                     2 
  6     WAITING    WAITING                     2 
  7*    RUN:cpu    WAITING          1          1 
  8*       DONE     RUN:io          1            
  9        DONE    WAITING                     1 
 10        DONE    WAITING                     1 
 11        DONE    WAITING                     1 
 12        DONE    WAITING                     1 
 13*       DONE    RUN:cpu          1            

Stats: Total Time 13
Stats: CPU Busy 6 (46.15%)
Stats: IO Busy  9 (69.23%)
```
## Case2 SWITCH_ON_IO VS. SWITCH_ON_END:
`./process-run.py -s 3 -l 3:50,3:50 -c -p -S SWITCH_ON_IO`

```
Time     PID: 0     PID: 1        CPU        IOs 
  1     RUN:cpu      READY          1            
  2      RUN:io      READY          1            
  3     WAITING     RUN:io          1          1 
  4     WAITING    WAITING                     2 
  5     WAITING    WAITING                     2 
  6     WAITING    WAITING                     2 
  7*    RUN:cpu    WAITING          1          1 
  8*       DONE     RUN:io          1            
  9        DONE    WAITING                     1 
 10        DONE    WAITING                     1 
 11        DONE    WAITING                     1 
 12        DONE    WAITING                     1 
 13*       DONE    RUN:cpu          1            

Stats: Total Time 13
Stats: CPU Busy 6 (46.15%)
Stats: IO Busy  9 (69.23%)
```
`./process-run.py -s 3 -l 3:50,3:50 -c -p -S SWITCH_ON_END`

```
Time     PID: 0     PID: 1        CPU        IOs 
  1     RUN:cpu      READY          1            
  2      RUN:io      READY          1            
  3     WAITING      READY                     1 
  4     WAITING      READY                     1 
  5     WAITING      READY                     1 
  6     WAITING      READY                     1 
  7*    RUN:cpu      READY          1            
  8        DONE     RUN:io          1            
  9        DONE    WAITING                     1 
 10        DONE    WAITING                     1 
 11        DONE    WAITING                     1 
 12        DONE    WAITING                     1 
 13*       DONE     RUN:io          1            
 14        DONE    WAITING                     1 
 15        DONE    WAITING                     1 
 16        DONE    WAITING                     1 
 17        DONE    WAITING                     1 
 18*       DONE    RUN:cpu          1            

Stats: Total Time 18
Stats: CPU Busy 6 (33.33%)
Stats: IO Busy  12 (66.67%)
```
