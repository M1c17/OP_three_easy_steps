# Homework (Simulation)

This program, mlfq.py, allows you to see how the MLFQ scheduler presented in this chapter behaves. See the README for details.

But first:
```
Usage: mlfq.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -n NUMQUEUES, --numQueues=NUMQUEUES
                        number of queues in MLFQ (if not using -Q)
  -q QUANTUM, --quantum=QUANTUM
                        length of time slice (if not using -Q)
  -a ALLOTMENT, --allotment=ALLOTMENT
                        length of allotment (if not using -A)
  -Q QUANTUMLIST, --quantumList=QUANTUMLIST
                        length of time slice per queue level, specified as
                        x,y,z,... where x is the quantum length for the
                        highest priority queue, y the next highest, and so
                        forth
  -A ALLOTMENTLIST, --allotmentList=ALLOTMENTLIST
                        length of time allotment per queue level, specified as
                        x,y,z,... where x is the # of time slices for the
                        highest priority queue, y the next highest, and so
                        forth
  -j NUMJOBS, --numJobs=NUMJOBS
                        number of jobs in the system
  -m MAXLEN, --maxlen=MAXLEN
                        max run-time of a job (if randomly generating)
  -M MAXIO, --maxio=MAXIO
                        max I/O frequency of a job (if randomly generating)
  -B BOOST, --boost=BOOST
                        how often to boost the priority of all jobs back to
                        high priority
  -i IOTIME, --iotime=IOTIME
                        how long an I/O should last (fixed constant)
  -S, --stay            reset and stay at same priority level when issuing I/O
  -I, --iobump          if specified, jobs that finished I/O move immediately
                        to front of current queue
  -l JLIST, --jlist=JLIST
                        a comma-separated list of jobs to run, in the form
                        x1,y1,z1:x2,y2,z2:... where x is start time, y is run
                        time, and z is how often the job issues an I/O request
  -c                    compute answers for me
```
Each job has two characteristics:
**run time:** how many time units it will run for. -R
**working set size:** how much cachespace it needs to run efficiently -

## Questions:
1. Run a few randomly-generated problems with just two jobs and two queues; compute the MLFQ execution trace for each. Make your life easier by limiting the length of each job and turning off I/Os.
```
$ ./mlfq.py -s 0 -n 2 -j 2 -m 20 -M 0 -c
```
```
Here is the list of inputs:
OPTIONS jobs 2
OPTIONS queues 2
OPTIONS allotments for queue  1 is   1
OPTIONS quantum length for queue  1 is  10
OPTIONS allotments for queue  0 is   1
OPTIONS quantum length for queue  0 is  10
OPTIONS boost 0
OPTIONS ioTime 5
OPTIONS stayAfterIO False
OPTIONS iobump False


For each job, three defining characteristics are given:
  startTime : at what time does the job enter the system
  runTime   : the total CPU time needed by the job to finish
  ioFreq    : every ioFreq time units, the job issues an I/O
              (the I/O takes ioTime units to complete)

Job List:
  Job  0: startTime   0 - runTime  17 - ioFreq   0
  Job  1: startTime   0 - runTime   8 - ioFreq   0


Execution Trace:

[ time 0 ] JOB BEGINS by JOB 0
[ time 0 ] JOB BEGINS by JOB 1
[ time 0 ] Run JOB 0 at PRIORITY 1 [ TICKS 9 ALLOT 1 TIME 16 (of 17) ]
[ time 1 ] Run JOB 0 at PRIORITY 1 [ TICKS 8 ALLOT 1 TIME 15 (of 17) ]
[ time 2 ] Run JOB 0 at PRIORITY 1 [ TICKS 7 ALLOT 1 TIME 14 (of 17) ]
[ time 3 ] Run JOB 0 at PRIORITY 1 [ TICKS 6 ALLOT 1 TIME 13 (of 17) ]
[ time 4 ] Run JOB 0 at PRIORITY 1 [ TICKS 5 ALLOT 1 TIME 12 (of 17) ]
[ time 5 ] Run JOB 0 at PRIORITY 1 [ TICKS 4 ALLOT 1 TIME 11 (of 17) ]
[ time 6 ] Run JOB 0 at PRIORITY 1 [ TICKS 3 ALLOT 1 TIME 10 (of 17) ]
[ time 7 ] Run JOB 0 at PRIORITY 1 [ TICKS 2 ALLOT 1 TIME 9 (of 17) ]
[ time 8 ] Run JOB 0 at PRIORITY 1 [ TICKS 1 ALLOT 1 TIME 8 (of 17) ]
[ time 9 ] Run JOB 0 at PRIORITY 1 [ TICKS 0 ALLOT 1 TIME 7 (of 17) ]
[ time 10 ] Run JOB 1 at PRIORITY 1 [ TICKS 9 ALLOT 1 TIME 7 (of 8) ]
[ time 11 ] Run JOB 1 at PRIORITY 1 [ TICKS 8 ALLOT 1 TIME 6 (of 8) ]
[ time 12 ] Run JOB 1 at PRIORITY 1 [ TICKS 7 ALLOT 1 TIME 5 (of 8) ]
[ time 13 ] Run JOB 1 at PRIORITY 1 [ TICKS 6 ALLOT 1 TIME 4 (of 8) ]
[ time 14 ] Run JOB 1 at PRIORITY 1 [ TICKS 5 ALLOT 1 TIME 3 (of 8) ]
[ time 15 ] Run JOB 1 at PRIORITY 1 [ TICKS 4 ALLOT 1 TIME 2 (of 8) ]
[ time 16 ] Run JOB 1 at PRIORITY 1 [ TICKS 3 ALLOT 1 TIME 1 (of 8) ]
[ time 17 ] Run JOB 1 at PRIORITY 1 [ TICKS 2 ALLOT 1 TIME 0 (of 8) ]
[ time 18 ] FINISHED JOB 1
[ time 18 ] Run JOB 0 at PRIORITY 0 [ TICKS 9 ALLOT 1 TIME 6 (of 17) ]
[ time 19 ] Run JOB 0 at PRIORITY 0 [ TICKS 8 ALLOT 1 TIME 5 (of 17) ]
[ time 20 ] Run JOB 0 at PRIORITY 0 [ TICKS 7 ALLOT 1 TIME 4 (of 17) ]
[ time 21 ] Run JOB 0 at PRIORITY 0 [ TICKS 6 ALLOT 1 TIME 3 (of 17) ]
[ time 22 ] Run JOB 0 at PRIORITY 0 [ TICKS 5 ALLOT 1 TIME 2 (of 17) ]
[ time 23 ] Run JOB 0 at PRIORITY 0 [ TICKS 4 ALLOT 1 TIME 1 (of 17) ]
[ time 24 ] Run JOB 0 at PRIORITY 0 [ TICKS 3 ALLOT 1 TIME 0 (of 17) ]
[ time 25 ] FINISHED JOB 0

Final statistics:
  Job  0: startTime   0 - response   0 - turnaround  25
  Job  1: startTime   0 - response  10 - turnaround  18

  Avg  1: startTime n/a - response 5.00 - turnaround 21.50
```
```
$ ./mlfq.py -s 1 -n 2 -j 2 -m 10 -M 0 -c
```
```
Here is the list of inputs:
OPTIONS jobs 2
OPTIONS queues 2
OPTIONS allotments for queue  1 is   1
OPTIONS quantum length for queue  1 is  10
OPTIONS allotments for queue  0 is   1
OPTIONS quantum length for queue  0 is  10
OPTIONS boost 0
OPTIONS ioTime 5
OPTIONS stayAfterIO False
OPTIONS iobump False


For each job, three defining characteristics are given:
  startTime : at what time does the job enter the system
  runTime   : the total CPU time needed by the job to finish
  ioFreq    : every ioFreq time units, the job issues an I/O
              (the I/O takes ioTime units to complete)

Job List:
  Job  0: startTime   0 - runTime   2 - ioFreq   0
  Job  1: startTime   0 - runTime   7 - ioFreq   0


Execution Trace:

[ time 0 ] JOB BEGINS by JOB 0
[ time 0 ] JOB BEGINS by JOB 1
[ time 0 ] Run JOB 0 at PRIORITY 1 [ TICKS 9 ALLOT 1 TIME 1 (of 2) ]
[ time 1 ] Run JOB 0 at PRIORITY 1 [ TICKS 8 ALLOT 1 TIME 0 (of 2) ]
[ time 2 ] FINISHED JOB 0
[ time 2 ] Run JOB 1 at PRIORITY 1 [ TICKS 9 ALLOT 1 TIME 6 (of 7) ]
[ time 3 ] Run JOB 1 at PRIORITY 1 [ TICKS 8 ALLOT 1 TIME 5 (of 7) ]
[ time 4 ] Run JOB 1 at PRIORITY 1 [ TICKS 7 ALLOT 1 TIME 4 (of 7) ]
[ time 5 ] Run JOB 1 at PRIORITY 1 [ TICKS 6 ALLOT 1 TIME 3 (of 7) ]
[ time 6 ] Run JOB 1 at PRIORITY 1 [ TICKS 5 ALLOT 1 TIME 2 (of 7) ]
[ time 7 ] Run JOB 1 at PRIORITY 1 [ TICKS 4 ALLOT 1 TIME 1 (of 7) ]
[ time 8 ] Run JOB 1 at PRIORITY 1 [ TICKS 3 ALLOT 1 TIME 0 (of 7) ]
[ time 9 ] FINISHED JOB 1

Final statistics:
  Job  0: startTime   0 - response   0 - turnaround   2
  Job  1: startTime   0 - response   2 - turnaround   9

  Avg  1: startTime n/a - response 1.00 - turnaround 5.50
```

2. How would you run the scheduler to reproduce each of the examples in the chapter?
```

// Example 1: A Single Long-Running Job
// Figure 8.2: Long-running Job Over Time

$ ./mlfq.py -n 3 -q 10 -l 0,200,0 -c

// Example 2: Along Came A Short Job
// Figure 8.3 Along Came An Interactive Job

$ ./mlfq.py -n 3 -l 0,200,0:100,20,0 -q 10 -c

// Example 3: What About I/O?
// Figure 8.4: A Mixed I/O-intensive and CPU-intensive Workload

$ ./mlfq.py -n 3 -l 0,185,0:50,20,1 -S -i 10 -c
$ ./mlfq.py -n 3 -l 0,185,0:50,20,1 -S -i 5 -c
$ ./mlfq.py -n 3 -l 0,185,0:50,20,1 -S -i 5 -Q 10,20,30 -c

// problem of starvation
// problem of game the scheduler

// Figure 8.5: Without Priority Boost

$ ./mlfq.py -n 3 -l 0,120,0:100,100,5:101,101,5 -q 10 -i 5 -S -c

// Figure 8.5: With Priority Boost

$ ./mlfq.py -n 3 -Q 20,10,30 -l 0,175,0:100,75,5:101,75,5 -S -B 50 -c

// Figure 8.6: Without Gaming Tolerance

$ ./mlfq.py -n 3 -q 10 -i 1 -l 0,260,0:80,170,10 -S -c

// Figure 8.6: With Gaming Tolerance

$ ./mlfq.py -n 3 -q 10 -i 1 -l 0,260,0:80,170,10 -c

// Figure 8.7: Lower Priority, Longer Quanta

$ ./mlfq.py -n 3 -a 1 -q 10 -l 0,220,0:20,270,0 -c

(You can separately control how much time allotment there is per queue too. This can be set for all queues with -a, or per queue with -A, e.g., -A 20,40,60 sets the time allotment per queue to 20ms, 40ms, and 60ms, respectively.)

```
3. How would you configure the scheduler parameters to behave just like a round-robin scheduler?

time slice <= (max job length / jobs number)

4. Craft a workload with two jobs and scheduler parameters so that one job takes advantage of the older Rules 4a and 4b (turned on with the -S flag) to game the scheduler and obtain 99% of the CPU over a particular time interval.
```
$ ./mlfq.py -n 3 -l 0,100,0:50,100,10 -S -i 1 -q 10 -c

```
5. Given a system with a quantum length of 10 ms in its highest queue, how often would you have to boost jobs back to the highest priority level (with the -B flag) in order to guarantee that a single longrunning (and potentially-starving) job gets at least 5% of the CPU?

Figure 8.5 (page 6)
"we at least guarantee that the long-running job will make some
progress, getting boosted to the highest priority every 50 ms and thus
getting to run periodically".

6. One question that arises in scheduling is which end of a queue to add a job that just finished I/O; the -I flag changes this behavior for this scheduling simulator. Play around with some workloads and see if you can see the effect of this flag.
```
-I, --iobump          if specified, jobs that finished I/O move immediately
```
```
$ ./mlfq.py -n 2 -q 10 -l 0,20,7:0,50,10 -i 1 -S -c
$ ./mlfq.py -n 2 -q 10 -l 0,20,7:0,50,10 -i 1 -S -I -c
$ ./mlfq.py -n 2 -q 10 -l 0,20,0:0,50,10 -i 1 -S -I -c
```



