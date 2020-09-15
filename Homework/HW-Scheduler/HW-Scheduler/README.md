# Homework (Simulation)
This program, scheduler.py, allows you to see how different schedulers perform under scheduling metrics such as response time, turnaround time, and total wait time. See the README for details.

Before we start:

```
$ ./scheduler.py -h
```
```
Usage: scheduler.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -j JOBS, --jobs=JOBS  number of jobs in the system
  -l JLIST, --jlist=JLIST
                        instead of random jobs, provide a comma-separated list
                        of run times
  -m MAXLEN, --maxlen=MAXLEN
                        max length of job
  -p POLICY, --policy=POLICY
                        sched policy to use: SJF, FIFO, RR
  -q QUANTUM, --quantum=QUANTUM
                        length of time slice for RR policy
  -c                    compute answers for me

```
Now we know how to use each flag in the program.
Now lets write some concepts:

**Response Time:**
Time from when the job arrives in a system to the first time is is scheduled
Time a job spends waiting after arrival before running

**Turnaround Time:**
Time it took to complete the job since first arrival

**Total wait time:**
Any time spent ready but not running 

Let's compute some policies running time jobs !!!

## Questions:

1. Compute the response time and turnaround time when running three jobs of length 200 with the SJF and FIFO schedulers
```
$ ./scheduler.py -p SJF -l 200,200,200 -c
```
```
ARG policy SJF
ARG jlist 200,200,200

Here is the job list, with the run time of each job: 
  Job 0 ( length = 200.0 )
  Job 1 ( length = 200.0 )
  Job 2 ( length = 200.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 200.00 secs ( DONE at 200.00 )
  [ time 200 ] Run job 1 for 200.00 secs ( DONE at 400.00 )
  [ time 400 ] Run job 2 for 200.00 secs ( DONE at 600.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 200.00  Wait 0.00
  Job   1 -- Response: 200.00  Turnaround 400.00  Wait 200.00
  Job   2 -- Response: 400.00  Turnaround 600.00  Wait 400.00

  Average -- Response: 200.00  Turnaround 400.00  Wait 200.00
```
```
$ ./scheduler.py -p FIFO -l 200,200,200 -c
```
```
ARG policy FIFO
ARG jlist 200,200,200

Here is the job list, with the run time of each job: 
  Job 0 ( length = 200.0 )
  Job 1 ( length = 200.0 )
  Job 2 ( length = 200.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 200.00 secs ( DONE at 200.00 )
  [ time 200 ] Run job 1 for 200.00 secs ( DONE at 400.00 )
  [ time 400 ] Run job 2 for 200.00 secs ( DONE at 600.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 200.00  Wait 0.00
  Job   1 -- Response: 200.00  Turnaround 400.00  Wait 200.00
  Job   2 -- Response: 400.00  Turnaround 600.00  Wait 400.00

  Average -- Response: 200.00  Turnaround 400.00  Wait 200.00
```

2. Now do the same but with jobs of different lengths: 100, 200, and 300.
```
$ ./scheduler.py -p SJF -l 100,200,300 -c
```
```
ARG policy SJF
ARG jlist 100,200,300

Here is the job list, with the run time of each job: 
  Job 0 ( length = 100.0 )
  Job 1 ( length = 200.0 )
  Job 2 ( length = 300.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 100.00 secs ( DONE at 100.00 )
  [ time 100 ] Run job 1 for 200.00 secs ( DONE at 300.00 )
  [ time 300 ] Run job 2 for 300.00 secs ( DONE at 600.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 100.00  Wait 0.00
  Job   1 -- Response: 100.00  Turnaround 300.00  Wait 100.00
  Job   2 -- Response: 300.00  Turnaround 600.00  Wait 300.00

  Average -- Response: 133.33  Turnaround 333.33  Wait 133.33
```
```
$ ./scheduler.py -p FIFO -l 100,200,300 -c
```
```
ARG policy FIFO
ARG jlist 100,200,300

Here is the job list, with the run time of each job: 
  Job 0 ( length = 100.0 )
  Job 1 ( length = 200.0 )
  Job 2 ( length = 300.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 100.00 secs ( DONE at 100.00 )
  [ time 100 ] Run job 1 for 200.00 secs ( DONE at 300.00 )
  [ time 300 ] Run job 2 for 300.00 secs ( DONE at 600.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 100.00  Wait 0.00
  Job   1 -- Response: 100.00  Turnaround 300.00  Wait 100.00
  Job   2 -- Response: 300.00  Turnaround 600.00  Wait 300.00

  Average -- Response: 133.33  Turnaround 333.33  Wait 133.33
```
3. Now do the same, but also with the RR scheduler and a time-slice of 1.
```
$ ./scheduler.py -p RR -l 20,20,20 -q 1 -c
```
I reduce the lenght of the jobs to 20, So my answer its not too long 
```
ARG policy RR
ARG jlist 20,20,20

Here is the job list, with the run time of each job: 
  Job 0 ( length = 20.0 )
  Job 1 ( length = 20.0 )
  Job 2 ( length = 20.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job   0 for 1.00 secs
  [ time   1 ] Run job   1 for 1.00 secs
  [ time   2 ] Run job   2 for 1.00 secs
  [ time   3 ] Run job   0 for 1.00 secs
  [ time   4 ] Run job   1 for 1.00 secs
  [ time   5 ] Run job   2 for 1.00 secs
  [ time   6 ] Run job   0 for 1.00 secs
  [ time   7 ] Run job   1 for 1.00 secs
  [ time   8 ] Run job   2 for 1.00 secs
  [ time   9 ] Run job   0 for 1.00 secs
  [ time  10 ] Run job   1 for 1.00 secs
  [ time  11 ] Run job   2 for 1.00 secs
  [ time  12 ] Run job   0 for 1.00 secs
  [ time  13 ] Run job   1 for 1.00 secs
  [ time  14 ] Run job   2 for 1.00 secs
  [ time  15 ] Run job   0 for 1.00 secs
  [ time  16 ] Run job   1 for 1.00 secs
  [ time  17 ] Run job   2 for 1.00 secs
  [ time  18 ] Run job   0 for 1.00 secs
  [ time  19 ] Run job   1 for 1.00 secs
  [ time  20 ] Run job   2 for 1.00 secs
  [ time  21 ] Run job   0 for 1.00 secs
  [ time  22 ] Run job   1 for 1.00 secs
  [ time  23 ] Run job   2 for 1.00 secs
  [ time  24 ] Run job   0 for 1.00 secs
  [ time  25 ] Run job   1 for 1.00 secs
  [ time  26 ] Run job   2 for 1.00 secs
  [ time  27 ] Run job   0 for 1.00 secs
  [ time  28 ] Run job   1 for 1.00 secs
  [ time  29 ] Run job   2 for 1.00 secs
  [ time  30 ] Run job   0 for 1.00 secs
  [ time  31 ] Run job   1 for 1.00 secs
  [ time  32 ] Run job   2 for 1.00 secs
  [ time  33 ] Run job   0 for 1.00 secs
  [ time  34 ] Run job   1 for 1.00 secs
  [ time  35 ] Run job   2 for 1.00 secs
  [ time  36 ] Run job   0 for 1.00 secs
  [ time  37 ] Run job   1 for 1.00 secs
  [ time  38 ] Run job   2 for 1.00 secs
  [ time  39 ] Run job   0 for 1.00 secs
  [ time  40 ] Run job   1 for 1.00 secs
  [ time  41 ] Run job   2 for 1.00 secs
  [ time  42 ] Run job   0 for 1.00 secs
  [ time  43 ] Run job   1 for 1.00 secs
  [ time  44 ] Run job   2 for 1.00 secs
  [ time  45 ] Run job   0 for 1.00 secs
  [ time  46 ] Run job   1 for 1.00 secs
  [ time  47 ] Run job   2 for 1.00 secs
  [ time  48 ] Run job   0 for 1.00 secs
  [ time  49 ] Run job   1 for 1.00 secs
  [ time  50 ] Run job   2 for 1.00 secs
  [ time  51 ] Run job   0 for 1.00 secs
  [ time  52 ] Run job   1 for 1.00 secs
  [ time  53 ] Run job   2 for 1.00 secs
  [ time  54 ] Run job   0 for 1.00 secs
  [ time  55 ] Run job   1 for 1.00 secs
  [ time  56 ] Run job   2 for 1.00 secs
  [ time  57 ] Run job   0 for 1.00 secs ( DONE at 58.00 )
  [ time  58 ] Run job   1 for 1.00 secs ( DONE at 59.00 )
  [ time  59 ] Run job   2 for 1.00 secs ( DONE at 60.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 58.00  Wait 38.00
  Job   1 -- Response: 1.00  Turnaround 59.00  Wait 39.00
  Job   2 -- Response: 2.00  Turnaround 60.00  Wait 40.00

  Average -- Response: 1.00  Turnaround 59.00  Wait 39.00
```
```
$ ./scheduler.py -p RR -l 10,20,30 -q 1 -c
```
I reduce the lenght of jobs to 10,20,30
```
ARG policy RR
ARG jlist 10,20,30

Here is the job list, with the run time of each job: 
  Job 0 ( length = 10.0 )
  Job 1 ( length = 20.0 )
  Job 2 ( length = 30.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job   0 for 1.00 secs
  [ time   1 ] Run job   1 for 1.00 secs
  [ time   2 ] Run job   2 for 1.00 secs
  [ time   3 ] Run job   0 for 1.00 secs
  [ time   4 ] Run job   1 for 1.00 secs
  [ time   5 ] Run job   2 for 1.00 secs
  [ time   6 ] Run job   0 for 1.00 secs
  [ time   7 ] Run job   1 for 1.00 secs
  [ time   8 ] Run job   2 for 1.00 secs
  [ time   9 ] Run job   0 for 1.00 secs
  [ time  10 ] Run job   1 for 1.00 secs
  [ time  11 ] Run job   2 for 1.00 secs
  [ time  12 ] Run job   0 for 1.00 secs
  [ time  13 ] Run job   1 for 1.00 secs
  [ time  14 ] Run job   2 for 1.00 secs
  [ time  15 ] Run job   0 for 1.00 secs
  [ time  16 ] Run job   1 for 1.00 secs
  [ time  17 ] Run job   2 for 1.00 secs
  [ time  18 ] Run job   0 for 1.00 secs
  [ time  19 ] Run job   1 for 1.00 secs
  [ time  20 ] Run job   2 for 1.00 secs
  [ time  21 ] Run job   0 for 1.00 secs
  [ time  22 ] Run job   1 for 1.00 secs
  [ time  23 ] Run job   2 for 1.00 secs
  [ time  24 ] Run job   0 for 1.00 secs
  [ time  25 ] Run job   1 for 1.00 secs
  [ time  26 ] Run job   2 for 1.00 secs
  [ time  27 ] Run job   0 for 1.00 secs ( DONE at 28.00 )
  [ time  28 ] Run job   1 for 1.00 secs
  [ time  29 ] Run job   2 for 1.00 secs
  [ time  30 ] Run job   1 for 1.00 secs
  [ time  31 ] Run job   2 for 1.00 secs
  [ time  32 ] Run job   1 for 1.00 secs
  [ time  33 ] Run job   2 for 1.00 secs
  [ time  34 ] Run job   1 for 1.00 secs
  [ time  35 ] Run job   2 for 1.00 secs
  [ time  36 ] Run job   1 for 1.00 secs
  [ time  37 ] Run job   2 for 1.00 secs
  [ time  38 ] Run job   1 for 1.00 secs
  [ time  39 ] Run job   2 for 1.00 secs
  [ time  40 ] Run job   1 for 1.00 secs
  [ time  41 ] Run job   2 for 1.00 secs
  [ time  42 ] Run job   1 for 1.00 secs
  [ time  43 ] Run job   2 for 1.00 secs
  [ time  44 ] Run job   1 for 1.00 secs
  [ time  45 ] Run job   2 for 1.00 secs
  [ time  46 ] Run job   1 for 1.00 secs
  [ time  47 ] Run job   2 for 1.00 secs
  [ time  48 ] Run job   1 for 1.00 secs ( DONE at 49.00 )
  [ time  49 ] Run job   2 for 1.00 secs
  [ time  50 ] Run job   2 for 1.00 secs
  [ time  51 ] Run job   2 for 1.00 secs
  [ time  52 ] Run job   2 for 1.00 secs
  [ time  53 ] Run job   2 for 1.00 secs
  [ time  54 ] Run job   2 for 1.00 secs
  [ time  55 ] Run job   2 for 1.00 secs
  [ time  56 ] Run job   2 for 1.00 secs
  [ time  57 ] Run job   2 for 1.00 secs
  [ time  58 ] Run job   2 for 1.00 secs
  [ time  59 ] Run job   2 for 1.00 secs ( DONE at 60.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 28.00  Wait 18.00
  Job   1 -- Response: 1.00  Turnaround 49.00  Wait 29.00
  Job   2 -- Response: 2.00  Turnaround 60.00  Wait 30.00

  Average -- Response: 1.00  Turnaround 45.67  Wait 25.67
```
4. For what types of workloads does SJF deliver the same turnaround times as FIFO?

As long as all jobs lenghts are either equal or arrive in ascending order, the turnaround times will be the same; except when a shorter job arrives after a longer job the turnaround will be different for SJF and FIFO. 

i.e:
```
$ ./scheduler.py -p SJF -l 30,10,20 -c
```
```
ARG policy SJF
ARG jlist 30,10,20

Here is the job list, with the run time of each job: 
  Job 0 ( length = 30.0 )
  Job 1 ( length = 10.0 )
  Job 2 ( length = 20.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 1 for 10.00 secs ( DONE at 10.00 )
  [ time  10 ] Run job 2 for 20.00 secs ( DONE at 30.00 )
  [ time  30 ] Run job 0 for 30.00 secs ( DONE at 60.00 )

Final statistics:
  Job   1 -- Response: 0.00  Turnaround 10.00  Wait 0.00
  Job   2 -- Response: 10.00  Turnaround 30.00  Wait 10.00
  Job   0 -- Response: 30.00  Turnaround 60.00  Wait 30.00

  Average -- Response: 13.33  Turnaround 33.33  Wait 13.33
```
As we can see above job 1 and job 2 had priority because SJF (Shortest Job First) perform that priority.
Average  Turnaround 33.33.

```
$ ./scheduler.py -p FIFO -l 30,10,20 -c
```
```
ARG policy FIFO
ARG jlist 30,10,20

Here is the job list, with the run time of each job: 
  Job 0 ( length = 30.0 )
  Job 1 ( length = 10.0 )
  Job 2 ( length = 20.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 30.00 secs ( DONE at 30.00 )
  [ time  30 ] Run job 1 for 10.00 secs ( DONE at 40.00 )
  [ time  40 ] Run job 2 for 20.00 secs ( DONE at 60.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 30.00  Wait 0.00
  Job   1 -- Response: 30.00  Turnaround 40.00  Wait 30.00
  Job   2 -- Response: 40.00  Turnaround 60.00  Wait 40.00

  Average -- Response: 23.33  Turnaround 43.33  Wait 23.33
```
Here FIFO run job 0, job 1, job 2 in ascending order.
FIFO (First in, First out)
FCFS (First come, First serve)

5. For what types of workloads and quantum lengths does SJF deliver the same response times as RR?

SJF deliver the same response time as RR as long as the quantum lenght will be the same as the jobs lenghts and all the jobs have the same lenght job.

i.e.:
```
$ ./scheduler.py -p SJF -l 10,10,10 -c 
```
```
ARG policy SJF
ARG jlist 10,10,10

Here is the job list, with the run time of each job: 
  Job 0 ( length = 10.0 )
  Job 1 ( length = 10.0 )
  Job 2 ( length = 10.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 10.00 secs ( DONE at 10.00 )
  [ time  10 ] Run job 1 for 10.00 secs ( DONE at 20.00 )
  [ time  20 ] Run job 2 for 10.00 secs ( DONE at 30.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 10.00  Wait 0.00
  Job   1 -- Response: 10.00  Turnaround 20.00  Wait 10.00
  Job   2 -- Response: 20.00  Turnaround 30.00  Wait 20.00

  Average -- Response: 10.00  Turnaround 20.00  Wait 10.00
```
```
$ ./scheduler.py -p RR -l 10,10,10 -c -q 10
```
```
ARG policy RR
ARG jlist 10,10,10

Here is the job list, with the run time of each job: 
  Job 0 ( length = 10.0 )
  Job 1 ( length = 10.0 )
  Job 2 ( length = 10.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job   0 for 10.00 secs ( DONE at 10.00 )
  [ time  10 ] Run job   1 for 10.00 secs ( DONE at 20.00 )
  [ time  20 ] Run job   2 for 10.00 secs ( DONE at 30.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 10.00  Wait 0.00
  Job   1 -- Response: 10.00  Turnaround 20.00  Wait 10.00
  Job   2 -- Response: 20.00  Turnaround 30.00  Wait 20.00

  Average -- Response: 10.00  Turnaround 20.00  Wait 10.00
```
6. What happens to response time with SJF as job lengths increase? Can you use the simulator to demonstrate the trend?
```
$ ./scheduler.py -p SJF -l 10,20,30 -c
```
```
ARG policy SJF
ARG jlist 10,20,30

Here is the job list, with the run time of each job: 
  Job 0 ( length = 10.0 )
  Job 1 ( length = 20.0 )
  Job 2 ( length = 30.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 10.00 secs ( DONE at 10.00 )
  [ time  10 ] Run job 1 for 20.00 secs ( DONE at 30.00 )
  [ time  30 ] Run job 2 for 30.00 secs ( DONE at 60.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 10.00  Wait 0.00
  Job   1 -- Response: 10.00  Turnaround 30.00  Wait 10.00
  Job   2 -- Response: 30.00  Turnaround 60.00  Wait 30.00

  Average -- Response: 13.33  Turnaround 33.33  Wait 13.33
```
```
$ ./scheduler.py -p SJF -l 30,40,60 -c
```
```
ARG policy SJF
ARG jlist 30,40,60

Here is the job list, with the run time of each job: 
  Job 0 ( length = 30.0 )
  Job 1 ( length = 40.0 )
  Job 2 ( length = 60.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 30.00 secs ( DONE at 30.00 )
  [ time  30 ] Run job 1 for 40.00 secs ( DONE at 70.00 )
  [ time  70 ] Run job 2 for 60.00 secs ( DONE at 130.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 30.00  Wait 0.00
  Job   1 -- Response: 30.00  Turnaround 70.00  Wait 30.00
  Job   2 -- Response: 70.00  Turnaround 130.00  Wait 70.00

  Average -- Response: 33.33  Turnaround 76.67  Wait 33.33
```
```
$ ./scheduler.py -p SJF -l 60,80,100 -c
```
```
ARG policy SJF
ARG jlist 60,80,100

Here is the job list, with the run time of each job: 
  Job 0 ( length = 60.0 )
  Job 1 ( length = 80.0 )
  Job 2 ( length = 100.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 60.00 secs ( DONE at 60.00 )
  [ time  60 ] Run job 1 for 80.00 secs ( DONE at 140.00 )
  [ time 140 ] Run job 2 for 100.00 secs ( DONE at 240.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 60.00  Wait 0.00
  Job   1 -- Response: 60.00  Turnaround 140.00  Wait 60.00
  Job   2 -- Response: 140.00  Turnaround 240.00  Wait 140.00

  Average -- Response: 66.67  Turnaround 146.67  Wait 66.67
```
If we doubles each job length, average response time also get double too. It mean there is linear relationship between job length and response time.

7. What happens to response time with RR as quantum lengths increase? Can you write an equation that gives the worst-case response time, given N jobs?
```
$ ./scheduler.py -p RR -l 50,20,10 -q 10 -c
```
```
ARG policy RR
ARG jlist 50,20,10

Here is the job list, with the run time of each job: 
  Job 0 ( length = 50.0 )
  Job 1 ( length = 20.0 )
  Job 2 ( length = 10.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job   0 for 10.00 secs
  [ time  10 ] Run job   1 for 10.00 secs
  [ time  20 ] Run job   2 for 10.00 secs ( DONE at 30.00 )
  [ time  30 ] Run job   0 for 10.00 secs
  [ time  40 ] Run job   1 for 10.00 secs ( DONE at 50.00 )
  [ time  50 ] Run job   0 for 10.00 secs
  [ time  60 ] Run job   0 for 10.00 secs
  [ time  70 ] Run job   0 for 10.00 secs ( DONE at 80.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 80.00  Wait 30.00
  Job   1 -- Response: 10.00  Turnaround 50.00  Wait 30.00
  Job   2 -- Response: 20.00  Turnaround 30.00  Wait 20.00

  Average -- Response: 10.00  Turnaround 53.33  Wait 26.67
```
```
$ ./scheduler.py -p RR -l 50,20,10 -q 100 -c
```
```
ARG policy RR
ARG jlist 50,20,10

Here is the job list, with the run time of each job: 
  Job 0 ( length = 50.0 )
  Job 1 ( length = 20.0 )
  Job 2 ( length = 10.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job   0 for 50.00 secs ( DONE at 50.00 )
  [ time  50 ] Run job   1 for 20.00 secs ( DONE at 70.00 )
  [ time  70 ] Run job   2 for 10.00 secs ( DONE at 80.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 50.00  Wait 0.00
  Job   1 -- Response: 50.00  Turnaround 70.00  Wait 50.00
  Job   2 -- Response: 70.00  Turnaround 80.00  Wait 70.00

  Average -- Response: 40.00  Turnaround 66.67  Wait 40.00
```
```
$ ./scheduler.py -p RR -l 80,90,70 -q 1000 -c
```
```
ARG policy RR
ARG jlist 80,90,70

Here is the job list, with the run time of each job: 
  Job 0 ( length = 80.0 )
  Job 1 ( length = 90.0 )
  Job 2 ( length = 70.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job   0 for 80.00 secs ( DONE at 80.00 )
  [ time  80 ] Run job   1 for 90.00 secs ( DONE at 170.00 )
  [ time 170 ] Run job   2 for 70.00 secs ( DONE at 240.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 80.00  Wait 0.00
  Job   1 -- Response: 80.00  Turnaround 170.00  Wait 80.00
  Job   2 -- Response: 170.00  Turnaround 240.00  Wait 170.00

  Average -- Response: 83.33  Turnaround 163.33  Wait 83.33
```
With RR, as quantum lengths increase response time increases because the time a process must wait for its next time slice (in the worst case) is proportional to the number of other processes and the maximum amount of time they can execute. Thus on a system with n processes and a quantum size of Q a process will have to wait for Q(n-1) to respond to a request (in the worst case)
