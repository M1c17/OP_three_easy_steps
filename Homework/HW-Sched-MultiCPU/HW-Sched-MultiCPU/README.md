# Homework (Simulation)
In this homework, we’ll use multi.py to simulate a multi-processor CPU scheduler, and learn about some of its details. Read the related README for more information about the simulator and its options.

But first:

```
$ ./multi.py -h
```

```
Usage: multi.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -j JOB_NUM, --job_num=JOB_NUM
                        number of jobs in the system
  -R MAX_RUN, --max_run=MAX_RUN
                        max run time of random-gen jobs
  -W MAX_WSET, --max_wset=MAX_WSET
                        max working set of random-gen jobs
  -L JOB_LIST, --job_list=JOB_LIST
                        provide a comma-separated list of
                        job_name:run_time:working_set_size (e.g.,
                        a:10:100,b:10:50 means 2 jobs with run-times of 10,
                        the first (a) with working set size=100, second (b)
                        with working set size=50)
  -p, --per_cpu_queues  per-CPU scheduling queues (not one)
  -A AFFINITY, --affinity=AFFINITY
                        a list of jobs and which CPUs they can run on (e.g.,
                        a:0.1.2,b:0.1 allows job a to run on CPUs 0,1,2 but b
                        only on CPUs 0 and 1
  -n NUM_CPUS, --num_cpus=NUM_CPUS
                        number of CPUs
  -q TIME_SLICE, --quantum=TIME_SLICE
                        length of time slice
  -P PEEK_INTERVAL, --peek_interval=PEEK_INTERVAL
                        for per-cpu scheduling, how often to peek at other
                        schedule queue; 0 turns this off
  -w WARMUP_TIME, --warmup_time=WARMUP_TIME
                        time it takes to warm cache
  -r WARM_RATE, --warm_rate=WARM_RATE
                        how much faster to run with warm cache
  -M CACHE_SIZE, --cache_size=CACHE_SIZE
                        cache size
  -o, --rand_order      has CPUs get jobs in random order
  -t, --trace           enable basic tracing (show which jobs got scheduled)
  -T, --trace_time_left
                        trace time left for each job
  -C, --trace_cache     trace cache status (warm/cold) too
  -S, --trace_sched     trace scheduler state
  -c, --compute         compute answers for me
```

## Questions:
1. To start things off, let’s learn how to use the simulator to study how to build an effective multi-processor scheduler. The first simulation will run just one job, which has a run-time of 30, and a working-set size of 200. Run this job (called job ’a’ here) on one simulated CPU as follows: ./multi.py -n 1 -L a:30:200. How long will it take to complete? Turn on the -c flag to see a final answer, and the -t flag to see a tick-by-tick trace of the job and how it is scheduled.

```
$ ./multi.py -n 1 -L a:30:200 -c -t
```
```
ARG seed 0
ARG job_num 3
ARG max_run 100
ARG max_wset 200
ARG job_list a:30:200
ARG affinity 
ARG per_cpu_queues False
ARG num_cpus 1
ARG quantum 10
ARG peek_interval 30
ARG warmup_time 10
ARG cache_size 100
ARG random_order False
ARG trace True
ARG trace_time False
ARG trace_cache False
ARG trace_sched False
ARG compute True

Job name:a run_time:30 working_set_size:200

Scheduler central queue: ['a']

   0   a      
   1   a      
   2   a      
   3   a      
   4   a      
   5   a      
   6   a      
   7   a      
   8   a      
   9   a      
----------
  10   a      
  11   a      
  12   a      
  13   a      
  14   a      
  15   a      
  16   a      
  17   a      
  18   a      
  19   a      
----------
  20   a      
  21   a      
  22   a      
  23   a      
  24   a      
  25   a      
  26   a      
  27   a      
  28   a      
  29   a      

Finished time 30

Per-CPU stats
  CPU 0  utilization 100.00 [ warm 0.00 ]
```

2. Now increase the cache size so as to make the job’s working set (size=200) fit into the cache (which, by default, is size=100); for example, run ./multi.py -n 1 -L a:30:200 -M 300. Can you predict how fast the job will run once it fits in cache? (hint: remember the key parameter of the warm rate, which is set by the -r flag) Check your answer by running with the solve flag (-c) enabled.

```
$ ./multi.py -n 1 -L a:30:200 -M 300 -c
```

```
ARG seed 0
ARG job_num 3
ARG max_run 100
ARG max_wset 200
ARG job_list a:30:200
ARG affinity 
ARG per_cpu_queues False
ARG num_cpus 1
ARG quantum 10
ARG peek_interval 30
ARG warmup_time 10
ARG cache_size 300
ARG random_order False
ARG trace False
ARG trace_time False
ARG trace_cache False
ARG trace_sched False
ARG compute True

Job name:a run_time:30 working_set_size:200

Scheduler central queue: ['a']


Finished time 20

Per-CPU stats
  CPU 0  utilization 100.00 [ warm 50.00 ]
```
Default warmup_time 10
10 time units for warming up cache
```
$ ./multi.py -n 1 -L a:30:200 -M 300 -r 20 -c
```

```
ARG seed 0
ARG job_num 3
ARG max_run 100
ARG max_wset 200
ARG job_list a:30:200
ARG affinity 
ARG per_cpu_queues False
ARG num_cpus 1
ARG quantum 10
ARG peek_interval 30
ARG warmup_time 10
ARG cache_size 300
ARG random_order False
ARG trace False
ARG trace_time False
ARG trace_cache False
ARG trace_sched False
ARG compute True

Job name:a run_time:30 working_set_size:200

Scheduler central queue: ['a']


Finished time 11

Per-CPU stats
  CPU 0  utilization 100.00 [ warm 9.09 ]
```
Run almost two time more faster.

3. One cool thing about multi.py is that you can see more detail about what is going on with different tracing flags. Run the same simulation as above, but this time with time left tracing enabled (-T). This flag shows both the job that was scheduled on a CPU at each time step, as well as how much run-time that job has left after each tick has run. What do you notice about how that second column decreases?

```
$ ./multi.py -n 1 -L a:30:200 -M 300 -c -T
```
```
ARG seed 0
ARG job_num 3
ARG max_run 100
ARG max_wset 200
ARG job_list a:30:200
ARG affinity 
ARG per_cpu_queues False
ARG num_cpus 1
ARG quantum 10
ARG peek_interval 30
ARG warmup_time 10
ARG cache_size 300
ARG random_order False
ARG trace False
ARG trace_time True
ARG trace_cache False
ARG trace_sched False
ARG compute True

Job name:a run_time:30 working_set_size:200

Scheduler central queue: ['a']

   0   a [ 29]      
   1   a [ 28]      
   2   a [ 27]      
   3   a [ 26]      
   4   a [ 25]      
   5   a [ 24]      
   6   a [ 23]      
   7   a [ 22]      
   8   a [ 21]      
   9   a [ 20]      
----------------
  10   a [ 18]      
  11   a [ 16]      
  12   a [ 14]      
  13   a [ 12]      
  14   a [ 10]      
  15   a [  8]      
  16   a [  6]      
  17   a [  4]      
  18   a [  2]      
  19   a [  0]      

Finished time 20

Per-CPU stats
  CPU 0  utilization 100.00 [ warm 50.00 ]
```

We can see above the first 10 time units will decrease 1 and after 10 will decrease 2.


4. Now add one more bit of tracing, to show the status of each CPU cache for each job, with the -C flag. For each job, each cache will either show a blank space (if the cache is cold for that job) or a ’w’ (if the cache is warm for that job). At what point does the cache become warm for job ’a’ in this simple example? What happens as you change the warmup time parameter (-w) to lower or higher values than the default?

```
$ ./multi.py -n 1 -L a:30:200 -M 300 -c -T -C
```

```
ARG seed 0
ARG job_num 3
ARG max_run 100
ARG max_wset 200
ARG job_list a:30:200
ARG affinity 
ARG per_cpu_queues False
ARG num_cpus 1
ARG quantum 10
ARG peek_interval 30
ARG warmup_time 10
ARG cache_size 300
ARG random_order False
ARG trace False
ARG trace_time True
ARG trace_cache True
ARG trace_sched False
ARG compute True

Job name:a run_time:30 working_set_size:200

Scheduler central queue: ['a']

   0   a [ 29] cache[ ]     
   1   a [ 28] cache[ ]     
   2   a [ 27] cache[ ]     
   3   a [ 26] cache[ ]     
   4   a [ 25] cache[ ]     
   5   a [ 24] cache[ ]     
   6   a [ 23] cache[ ]     
   7   a [ 22] cache[ ]     
   8   a [ 21] cache[ ]     
   9   a [ 20] cache[w]     
-------------------------
  10   a [ 18] cache[w]     
  11   a [ 16] cache[w]     
  12   a [ 14] cache[w]     
  13   a [ 12] cache[w]     
  14   a [ 10] cache[w]     
  15   a [  8] cache[w]     
  16   a [  6] cache[w]     
  17   a [  4] cache[w]     
  18   a [  2] cache[w]     
  19   a [  0] cache[w]     

Finished time 20

Per-CPU stats
  CPU 0  utilization 100.00 [ warm 50.00 ]
```

```
$ ./multi.py -n 1 -L a:30:200 -M 300 -c -T -C -w 20
```
```
ARG seed 0
ARG job_num 3
ARG max_run 100
ARG max_wset 200
ARG job_list a:30:200
ARG affinity 
ARG per_cpu_queues False
ARG num_cpus 1
ARG quantum 10
ARG peek_interval 30
ARG warmup_time 20
ARG cache_size 300
ARG random_order False
ARG trace False
ARG trace_time True
ARG trace_cache True
ARG trace_sched False
ARG compute True

Job name:a run_time:30 working_set_size:200

Scheduler central queue: ['a']

   0   a [ 29] cache[ ]     
   1   a [ 28] cache[ ]     
   2   a [ 27] cache[ ]     
   3   a [ 26] cache[ ]     
   4   a [ 25] cache[ ]     
   5   a [ 24] cache[ ]     
   6   a [ 23] cache[ ]     
   7   a [ 22] cache[ ]     
   8   a [ 21] cache[ ]     
   9   a [ 20] cache[ ]     
-------------------------
  10   a [ 19] cache[ ]     
  11   a [ 18] cache[ ]     
  12   a [ 17] cache[ ]     
  13   a [ 16] cache[ ]     
  14   a [ 15] cache[ ]     
  15   a [ 14] cache[ ]     
  16   a [ 13] cache[ ]     
  17   a [ 12] cache[ ]     
  18   a [ 11] cache[ ]     
  19   a [ 10] cache[w]     
-------------------------
  20   a [  8] cache[w]     
  21   a [  6] cache[w]     
  22   a [  4] cache[w]     
  23   a [  2] cache[w]     
  24   a [  0] cache[w]     

Finished time 25

Per-CPU stats
  CPU 0  utilization 100.00 [ warm 20.00 ]
```

5. At this point, you should have a good idea of how the simulator works for a single job running on a single CPU. But hey, isn’t this a multi-processor CPU scheduling chapter? Oh yeah! So let’s start working with multiple jobs. Specifically, let’s run the following three jobs on a two-CPU system (i.e., type ./multi.py -n 2 -L a:100:100,b:100:50,c:100:50) Can you predict how long this will take, given a round-robin centralized scheduler? Use -c to see if you were right, and then dive down into details with -t to see a step-by-step and then -C to see whether caches got warmed effectively for these jobs. What do you notice?
```
$ ./multi.py -n 2 -L a:100:100,b:100:50,c:100:50 -t -c -C
```

```
ARG seed 0
ARG job_num 3
ARG max_run 100
ARG max_wset 200
ARG job_list a:100:100,b:100:50,c:100:50
ARG affinity 
ARG per_cpu_queues False
ARG num_cpus 2
ARG quantum 10
ARG peek_interval 30
ARG warmup_time 10
ARG cache_size 100
ARG random_order False
ARG trace True
ARG trace_time False
ARG trace_cache True
ARG trace_sched False
ARG compute True

Job name:a run_time:100 working_set_size:100
Job name:b run_time:100 working_set_size:50
Job name:c run_time:100 working_set_size:50

Scheduler central queue: ['a', 'b', 'c']

   0   a cache[   ]     b cache[   ]     
   1   a cache[   ]     b cache[   ]     
   2   a cache[   ]     b cache[   ]     
   3   a cache[   ]     b cache[   ]     
   4   a cache[   ]     b cache[   ]     
   5   a cache[   ]     b cache[   ]     
   6   a cache[   ]     b cache[   ]     
   7   a cache[   ]     b cache[   ]     
   8   a cache[   ]     b cache[   ]     
   9   a cache[w  ]     b cache[ w ]     
---------------------------------------
  10   c cache[w  ]     a cache[ w ]     
  11   c cache[w  ]     a cache[ w ]     
  12   c cache[w  ]     a cache[ w ]     
  13   c cache[w  ]     a cache[ w ]     
  14   c cache[w  ]     a cache[ w ]     
  15   c cache[w  ]     a cache[ w ]     
  16   c cache[w  ]     a cache[ w ]     
  17   c cache[w  ]     a cache[ w ]     
  18   c cache[w  ]     a cache[ w ]     
  19   c cache[  w]     a cache[w  ]     
---------------------------------------
  20   b cache[  w]     c cache[w  ]     
  21   b cache[  w]     c cache[w  ]     
  22   b cache[  w]     c cache[w  ]     
  23   b cache[  w]     c cache[w  ]     
  24   b cache[  w]     c cache[w  ]     
  25   b cache[  w]     c cache[w  ]     
  26   b cache[  w]     c cache[w  ]     
  27   b cache[  w]     c cache[w  ]     
  28   b cache[  w]     c cache[w  ]     
  29   b cache[ ww]     c cache[  w]     
---------------------------------------
  30   a cache[ ww]     b cache[  w]     
  31   a cache[ ww]     b cache[  w]     
  32   a cache[ ww]     b cache[  w]     
  33   a cache[ ww]     b cache[  w]     
  34   a cache[ ww]     b cache[  w]     
  35   a cache[ ww]     b cache[  w]     
  36   a cache[ ww]     b cache[  w]     
  37   a cache[ ww]     b cache[  w]     
  38   a cache[ ww]     b cache[  w]     
  39   a cache[w  ]     b cache[ ww]     
---------------------------------------
  40   c cache[w  ]     a cache[ ww]     
  41   c cache[w  ]     a cache[ ww]     
  42   c cache[w  ]     a cache[ ww]     
  43   c cache[w  ]     a cache[ ww]     
  44   c cache[w  ]     a cache[ ww]     
  45   c cache[w  ]     a cache[ ww]     
  46   c cache[w  ]     a cache[ ww]     
  47   c cache[w  ]     a cache[ ww]     
  48   c cache[w  ]     a cache[ ww]     
  49   c cache[  w]     a cache[w  ]     
---------------------------------------
  50   b cache[  w]     c cache[w  ]     
  51   b cache[  w]     c cache[w  ]     
  52   b cache[  w]     c cache[w  ]     
  53   b cache[  w]     c cache[w  ]     
  54   b cache[  w]     c cache[w  ]     
  55   b cache[  w]     c cache[w  ]     
  56   b cache[  w]     c cache[w  ]     
  57   b cache[  w]     c cache[w  ]     
  58   b cache[  w]     c cache[w  ]     
  59   b cache[ ww]     c cache[  w]     
---------------------------------------
  60   a cache[ ww]     b cache[  w]     
  61   a cache[ ww]     b cache[  w]     
  62   a cache[ ww]     b cache[  w]     
  63   a cache[ ww]     b cache[  w]     
  64   a cache[ ww]     b cache[  w]     
  65   a cache[ ww]     b cache[  w]     
  66   a cache[ ww]     b cache[  w]     
  67   a cache[ ww]     b cache[  w]     
  68   a cache[ ww]     b cache[  w]     
  69   a cache[w  ]     b cache[ ww]     
---------------------------------------
  70   c cache[w  ]     a cache[ ww]     
  71   c cache[w  ]     a cache[ ww]     
  72   c cache[w  ]     a cache[ ww]     
  73   c cache[w  ]     a cache[ ww]     
  74   c cache[w  ]     a cache[ ww]     
  75   c cache[w  ]     a cache[ ww]     
  76   c cache[w  ]     a cache[ ww]     
  77   c cache[w  ]     a cache[ ww]     
  78   c cache[w  ]     a cache[ ww]     
  79   c cache[  w]     a cache[w  ]     
---------------------------------------
  80   b cache[  w]     c cache[w  ]     
  81   b cache[  w]     c cache[w  ]     
  82   b cache[  w]     c cache[w  ]     
  83   b cache[  w]     c cache[w  ]     
  84   b cache[  w]     c cache[w  ]     
  85   b cache[  w]     c cache[w  ]     
  86   b cache[  w]     c cache[w  ]     
  87   b cache[  w]     c cache[w  ]     
  88   b cache[  w]     c cache[w  ]     
  89   b cache[ ww]     c cache[  w]     
---------------------------------------
  90   a cache[ ww]     b cache[  w]     
  91   a cache[ ww]     b cache[  w]     
  92   a cache[ ww]     b cache[  w]     
  93   a cache[ ww]     b cache[  w]     
  94   a cache[ ww]     b cache[  w]     
  95   a cache[ ww]     b cache[  w]     
  96   a cache[ ww]     b cache[  w]     
  97   a cache[ ww]     b cache[  w]     
  98   a cache[ ww]     b cache[  w]     
  99   a cache[w  ]     b cache[ ww]     
---------------------------------------
 100   c cache[w  ]     a cache[ ww]     
 101   c cache[w  ]     a cache[ ww]     
 102   c cache[w  ]     a cache[ ww]     
 103   c cache[w  ]     a cache[ ww]     
 104   c cache[w  ]     a cache[ ww]     
 105   c cache[w  ]     a cache[ ww]     
 106   c cache[w  ]     a cache[ ww]     
 107   c cache[w  ]     a cache[ ww]     
 108   c cache[w  ]     a cache[ ww]     
 109   c cache[  w]     a cache[w  ]     
---------------------------------------
 110   b cache[  w]     c cache[w  ]     
 111   b cache[  w]     c cache[w  ]     
 112   b cache[  w]     c cache[w  ]     
 113   b cache[  w]     c cache[w  ]     
 114   b cache[  w]     c cache[w  ]     
 115   b cache[  w]     c cache[w  ]     
 116   b cache[  w]     c cache[w  ]     
 117   b cache[  w]     c cache[w  ]     
 118   b cache[  w]     c cache[w  ]     
 119   b cache[ ww]     c cache[  w]     
---------------------------------------
 120   a cache[ ww]     b cache[  w]     
 121   a cache[ ww]     b cache[  w]     
 122   a cache[ ww]     b cache[  w]     
 123   a cache[ ww]     b cache[  w]     
 124   a cache[ ww]     b cache[  w]     
 125   a cache[ ww]     b cache[  w]     
 126   a cache[ ww]     b cache[  w]     
 127   a cache[ ww]     b cache[  w]     
 128   a cache[ ww]     b cache[  w]     
 129   a cache[w  ]     b cache[ ww]     
---------------------------------------
 130   c cache[w  ]     a cache[ ww]     
 131   c cache[w  ]     a cache[ ww]     
 132   c cache[w  ]     a cache[ ww]     
 133   c cache[w  ]     a cache[ ww]     
 134   c cache[w  ]     a cache[ ww]     
 135   c cache[w  ]     a cache[ ww]     
 136   c cache[w  ]     a cache[ ww]     
 137   c cache[w  ]     a cache[ ww]     
 138   c cache[w  ]     a cache[ ww]     
 139   c cache[  w]     a cache[w  ]     
---------------------------------------
 140   b cache[  w]     c cache[w  ]     
 141   b cache[  w]     c cache[w  ]     
 142   b cache[  w]     c cache[w  ]     
 143   b cache[  w]     c cache[w  ]     
 144   b cache[  w]     c cache[w  ]     
 145   b cache[  w]     c cache[w  ]     
 146   b cache[  w]     c cache[w  ]     
 147   b cache[  w]     c cache[w  ]     
 148   b cache[  w]     c cache[w  ]     
 149   b cache[ ww]     c cache[  w]     

Finished time 150

Per-CPU stats
  CPU 0  utilization 100.00 [ warm 0.00 ]
  CPU 1  utilization 100.00 [ warm 0.00 ]
```
Caches get cleared every 10 time units, it never run faster, 300/2 = 150.

6. Now we’ll apply some explicit controls to study cache affinity, as described in the chapter. To do this, you’ll need the -A flag. This flag can be used to limit which CPUs the scheduler can place a particular job upon. In this case, let’s use it to place jobs ’b’ and ’c’ on CPU 1, while restricting ’a’ to CPU 0. This magic is accomplished by typing this ./multi.py -n 2 -L a:100:100,b:100:50, c:100:50 -A a:0,b:1,c:1 ; don’t forget to turn on various tracing options to see what is really happening! Can you predict how fast this version will run? Why does it do better? Will other combinations of ’a’, ’b’, and ’c’ onto the two processors run faster or slower?
```
$ ./multi.py -n 2 -L a:100:100,b:100:50,c:100:50 -A a:0,b:1,c:1 -t -T -C -c
```
```
ARG seed 0
ARG job_num 3
ARG max_run 100
ARG max_wset 200
ARG job_list a:100:100,b:100:50,c:100:50
ARG affinity a:0,b:1,c:1
ARG per_cpu_queues False
ARG num_cpus 2
ARG quantum 10
ARG peek_interval 30
ARG warmup_time 10
ARG cache_size 100
ARG random_order False
ARG trace True
ARG trace_time True
ARG trace_cache True
ARG trace_sched False
ARG compute True

Job name:a run_time:100 working_set_size:100
Job name:b run_time:100 working_set_size:50
Job name:c run_time:100 working_set_size:50

Scheduler central queue: ['a', 'b', 'c']

   0   a [ 99] cache[   ]     b [ 99] cache[   ]     
   1   a [ 98] cache[   ]     b [ 98] cache[   ]     
   2   a [ 97] cache[   ]     b [ 97] cache[   ]     
   3   a [ 96] cache[   ]     b [ 96] cache[   ]     
   4   a [ 95] cache[   ]     b [ 95] cache[   ]     
   5   a [ 94] cache[   ]     b [ 94] cache[   ]     
   6   a [ 93] cache[   ]     b [ 93] cache[   ]     
   7   a [ 92] cache[   ]     b [ 92] cache[   ]     
   8   a [ 91] cache[   ]     b [ 91] cache[   ]     
   9   a [ 90] cache[w  ]     b [ 90] cache[ w ]     
---------------------------------------------------
  10   a [ 88] cache[w  ]     c [ 99] cache[ w ]     
  11   a [ 86] cache[w  ]     c [ 98] cache[ w ]     
  12   a [ 84] cache[w  ]     c [ 97] cache[ w ]     
  13   a [ 82] cache[w  ]     c [ 96] cache[ w ]     
  14   a [ 80] cache[w  ]     c [ 95] cache[ w ]     
  15   a [ 78] cache[w  ]     c [ 94] cache[ w ]     
  16   a [ 76] cache[w  ]     c [ 93] cache[ w ]     
  17   a [ 74] cache[w  ]     c [ 92] cache[ w ]     
  18   a [ 72] cache[w  ]     c [ 91] cache[ w ]     
  19   a [ 70] cache[w  ]     c [ 90] cache[ ww]     
---------------------------------------------------
  20   a [ 68] cache[w  ]     b [ 88] cache[ ww]     
  21   a [ 66] cache[w  ]     b [ 86] cache[ ww]     
  22   a [ 64] cache[w  ]     b [ 84] cache[ ww]     
  23   a [ 62] cache[w  ]     b [ 82] cache[ ww]     
  24   a [ 60] cache[w  ]     b [ 80] cache[ ww]     
  25   a [ 58] cache[w  ]     b [ 78] cache[ ww]     
  26   a [ 56] cache[w  ]     b [ 76] cache[ ww]     
  27   a [ 54] cache[w  ]     b [ 74] cache[ ww]     
  28   a [ 52] cache[w  ]     b [ 72] cache[ ww]     
  29   a [ 50] cache[w  ]     b [ 70] cache[ ww]     
---------------------------------------------------
  30   a [ 48] cache[w  ]     c [ 88] cache[ ww]     
  31   a [ 46] cache[w  ]     c [ 86] cache[ ww]     
  32   a [ 44] cache[w  ]     c [ 84] cache[ ww]     
  33   a [ 42] cache[w  ]     c [ 82] cache[ ww]     
  34   a [ 40] cache[w  ]     c [ 80] cache[ ww]     
  35   a [ 38] cache[w  ]     c [ 78] cache[ ww]     
  36   a [ 36] cache[w  ]     c [ 76] cache[ ww]     
  37   a [ 34] cache[w  ]     c [ 74] cache[ ww]     
  38   a [ 32] cache[w  ]     c [ 72] cache[ ww]     
  39   a [ 30] cache[w  ]     c [ 70] cache[ ww]     
---------------------------------------------------
  40   a [ 28] cache[w  ]     b [ 68] cache[ ww]     
  41   a [ 26] cache[w  ]     b [ 66] cache[ ww]     
  42   a [ 24] cache[w  ]     b [ 64] cache[ ww]     
  43   a [ 22] cache[w  ]     b [ 62] cache[ ww]     
  44   a [ 20] cache[w  ]     b [ 60] cache[ ww]     
  45   a [ 18] cache[w  ]     b [ 58] cache[ ww]     
  46   a [ 16] cache[w  ]     b [ 56] cache[ ww]     
  47   a [ 14] cache[w  ]     b [ 54] cache[ ww]     
  48   a [ 12] cache[w  ]     b [ 52] cache[ ww]     
  49   a [ 10] cache[w  ]     b [ 50] cache[ ww]     
---------------------------------------------------
  50   a [  8] cache[w  ]     c [ 68] cache[ ww]     
  51   a [  6] cache[w  ]     c [ 66] cache[ ww]     
  52   a [  4] cache[w  ]     c [ 64] cache[ ww]     
  53   a [  2] cache[w  ]     c [ 62] cache[ ww]     
  54   a [  0] cache[w  ]     c [ 60] cache[ ww]     
  55   - [   ] cache[w  ]     c [ 58] cache[ ww]     
  56   - [   ] cache[w  ]     c [ 56] cache[ ww]     
  57   - [   ] cache[w  ]     c [ 54] cache[ ww]     
  58   - [   ] cache[w  ]     c [ 52] cache[ ww]     
  59   - [   ] cache[w  ]     c [ 50] cache[ ww]     
---------------------------------------------------
  60   - [   ] cache[w  ]     b [ 48] cache[ ww]     
  61   - [   ] cache[w  ]     b [ 46] cache[ ww]     
  62   - [   ] cache[w  ]     b [ 44] cache[ ww]     
  63   - [   ] cache[w  ]     b [ 42] cache[ ww]     
  64   - [   ] cache[w  ]     b [ 40] cache[ ww]     
  65   - [   ] cache[w  ]     b [ 38] cache[ ww]     
  66   - [   ] cache[w  ]     b [ 36] cache[ ww]     
  67   - [   ] cache[w  ]     b [ 34] cache[ ww]     
  68   - [   ] cache[w  ]     b [ 32] cache[ ww]     
  69   - [   ] cache[w  ]     b [ 30] cache[ ww]     
---------------------------------------------------
  70   - [   ] cache[w  ]     c [ 48] cache[ ww]     
  71   - [   ] cache[w  ]     c [ 46] cache[ ww]     
  72   - [   ] cache[w  ]     c [ 44] cache[ ww]     
  73   - [   ] cache[w  ]     c [ 42] cache[ ww]     
  74   - [   ] cache[w  ]     c [ 40] cache[ ww]     
  75   - [   ] cache[w  ]     c [ 38] cache[ ww]     
  76   - [   ] cache[w  ]     c [ 36] cache[ ww]     
  77   - [   ] cache[w  ]     c [ 34] cache[ ww]     
  78   - [   ] cache[w  ]     c [ 32] cache[ ww]     
  79   - [   ] cache[w  ]     c [ 30] cache[ ww]     
---------------------------------------------------
  80   - [   ] cache[w  ]     b [ 28] cache[ ww]     
  81   - [   ] cache[w  ]     b [ 26] cache[ ww]     
  82   - [   ] cache[w  ]     b [ 24] cache[ ww]     
  83   - [   ] cache[w  ]     b [ 22] cache[ ww]     
  84   - [   ] cache[w  ]     b [ 20] cache[ ww]     
  85   - [   ] cache[w  ]     b [ 18] cache[ ww]     
  86   - [   ] cache[w  ]     b [ 16] cache[ ww]     
  87   - [   ] cache[w  ]     b [ 14] cache[ ww]     
  88   - [   ] cache[w  ]     b [ 12] cache[ ww]     
  89   - [   ] cache[w  ]     b [ 10] cache[ ww]     
---------------------------------------------------
  90   - [   ] cache[w  ]     c [ 28] cache[ ww]     
  91   - [   ] cache[w  ]     c [ 26] cache[ ww]     
  92   - [   ] cache[w  ]     c [ 24] cache[ ww]     
  93   - [   ] cache[w  ]     c [ 22] cache[ ww]     
  94   - [   ] cache[w  ]     c [ 20] cache[ ww]     
  95   - [   ] cache[w  ]     c [ 18] cache[ ww]     
  96   - [   ] cache[w  ]     c [ 16] cache[ ww]     
  97   - [   ] cache[w  ]     c [ 14] cache[ ww]     
  98   - [   ] cache[w  ]     c [ 12] cache[ ww]     
  99   - [   ] cache[w  ]     c [ 10] cache[ ww]     
---------------------------------------------------
 100   - [   ] cache[w  ]     b [  8] cache[ ww]     
 101   - [   ] cache[w  ]     b [  6] cache[ ww]     
 102   - [   ] cache[w  ]     b [  4] cache[ ww]     
 103   - [   ] cache[w  ]     b [  2] cache[ ww]     
 104   - [   ] cache[w  ]     b [  0] cache[ ww]     
 105   - [   ] cache[w  ]     c [  8] cache[ ww]     
 106   - [   ] cache[w  ]     c [  6] cache[ ww]     
 107   - [   ] cache[w  ]     c [  4] cache[ ww]     
 108   - [   ] cache[w  ]     c [  2] cache[ ww]     
 109   - [   ] cache[w  ]     c [  0] cache[ ww]     

Finished time 110

Per-CPU stats
  CPU 0  utilization 50.00 [ warm 40.91 ]
  CPU 1  utilization 100.00 [ warm 81.82 ]
```
Here we arrive with the problem of load imbalance. Where job a get twice as much CPU as b and c finished first and what is worst leaving idle CPU 0 this is a poor poor multi-queue multiprocessor bad utilization of CPU.


7. One interesting aspect of caching multiprocessors is the opportunity for better-than-expected speed up of jobs when using multiple CPUs (and their caches) as compared to running jobs on a single processor. Specifically, when you run on N CPUs, sometimes you can speed up by more than a factor of N, a situation entitled super-linear speedup. To experiment with this, use the job description here (-L a:100:100,b:100:100,c:100:100) with a small cache (-M 50) to create three jobs. Run this on systems with 1, 2, and 3 CPUs (-n 1, -n 2, -n 3). Now, do the same, but with a larger per-CPU cache of size 100. What do you notice about performance as the number of CPUs scales? Use -c to confirm your guesses, and other tracing flags to dive even deeper.
```
$ ./multi.py -n 1 -L a:100:100,b:100:100,c:100:100 -M 50 -t -T -C -c
```
no cache
```
Finished time 300

Per-CPU stats
  CPU 0  utilization 100.00 [ warm 0.00 ]
```
```
$ ./multi.py -n 2 -L a:100:100,b:100:100,c:100:100 -M 50 -t -T -C -c
```
no cache
```
Finished time 150

Per-CPU stats
  CPU 0  utilization 100.00 [ warm 0.00 ]
  CPU 1  utilization 100.00 [ warm 0.00 ]
```
```
$ ./multi.py -n 3 -L a:100:100,b:100:100,c:100:100 -M 50 -t -T -C -c
```
no cache
```
Finished time 100

Per-CPU stats
  CPU 0  utilization 100.00 [ warm 0.00 ]
  CPU 1  utilization 100.00 [ warm 0.00 ]
  CPU 2  utilization 100.00 [ warm 0.00 ]
```
Here we can observe as the number of CPUs scales the time is less we had good performance

```
$ ./multi.py -n 1 -L a:100:100,b:100:100,c:100:100 -M 100 -t -T -C -c
```
no cache
```
Finished time 300

Per-CPU stats
  CPU 0  utilization 100.00 [ warm 0.00 ]
```
```
$ ./multi.py -n 2 -L a:100:100,b:100:100,c:100:100 -M 100 -t -T -C -c
```
no cache
```
Finished time 150

Per-CPU stats
  CPU 0  utilization 100.00 [ warm 0.00 ]
  CPU 1  utilization 100.00 [ warm 0.00 ]

```
```
$ ./multi.py -n 3 -L a:100:100,b:100:100,c:100:100 -M 100 -t -T -C -c
```
cache (10 + 90/2 = 55)
```
Finished time 55

Per-CPU stats
  CPU 0  utilization 100.00 [ warm 81.82 ]
  CPU 1  utilization 100.00 [ warm 81.82 ]
  CPU 2  utilization 100.00 [ warm 81.82 ]
```
Same.
better performance with cache

8. One other aspect of the simulator worth studying is the per-CPU scheduling option, the -p flag. Run with two CPUs again, and this three job configuration (-L a:100:100,b:100:50,c:100:50). How does this option do, as opposed to the hand-controlled affinity limits you put in place above? How does performance change as you alter the ’peek interval’ (-P) to lower or higher values? How does this per-CPU approach work as the number of CPUs scales?

-p, --per_cpu_queues  per-CPU scheduling queues (not one)
-P PEEK_INTERVAL, --peek_interval=PEEK_INTERVAL
for per-cpu scheduling, how often to peek at other
schedule queue; 0 turns this off

```
$ ./multi.py -n 2 -L a:100:100,b:100:50,c:100:50 -p -c -t -T -C
```
```
ARG per_cpu_queues True
Scheduler CPU 0 queue: ['a', 'c']
Scheduler CPU 1 queue: ['b']

Finished time 100

Per-CPU stats
  CPU 0  utilization 95.00 [ warm 35.00 ]
  CPU 1  utilization 95.00 [ warm 75.00 ]
```
```
$ ./multi.py -n 2 -L a:100:100,b:100:100,c:100:100 -p -c -t -T -C -P 10 
```
```
ARG per_cpu_queues True
Scheduler CPU 0 queue: ['a', 'c']
Scheduler CPU 1 queue: ['b']

Finished time 100

Per-CPU stats
  CPU 0  utilization 95.00 [ warm 35.00 ]
  CPU 1  utilization 95.00 [ warm 75.00 ]
```
```
$ ./multi.py -n 2 -L a:100:100,b:100:100,c:100:100 -p -c -t -T -C -P 40 
```
```
ARG per_cpu_queues True
ARG peek_interval 40

Scheduler CPU 0 queue: ['a', 'c']
Scheduler CPU 1 queue: ['b']

Finished time 115

Per-CPU stats
  CPU 0  utilization 95.65 [ warm 26.09 ]
  CPU 1  utilization 78.26 [ warm 60.87 ]
```
```
$ ./multi.py -n 3 -L a:100:100,b:100:100,c:100:100 -p -c -t -T -C 
```
```
ARG per_cpu_queues True
ARG peek_interval 40

Scheduler CPU 0 queue: ['a']
Scheduler CPU 1 queue: ['b']
Scheduler CPU 2 queue: ['c']

Finished time 55

Per-CPU stats
  CPU 0  utilization 100.00 [ warm 81.82 ]
  CPU 1  utilization 100.00 [ warm 81.82 ]
  CPU 2  utilization 100.00 [ warm 81.82 ]
```
```
$ ./multi.py -n 4 -L a:100:100,b:100:100,c:100:100 -p -c -t -T -C 
```
```
ARG per_cpu_queues True
ARG peek_interval 30

Scheduler CPU 0 queue: ['a']
Scheduler CPU 1 queue: ['b']
Scheduler CPU 2 queue: ['c']
Scheduler CPU 3 queue: []

Finished time 60

Per-CPU stats
  CPU 0  utilization 91.67 [ warm 75.00 ]
  CPU 1  utilization 50.00 [ warm 33.33 ]
  CPU 2  utilization 91.67 [ warm 75.00 ]
  CPU 3  utilization 50.00 [ warm 33.33 ]
```
```
$ ./multi.py -n 5 -L a:100:100,b:100:100,c:100:100 -p -c -t -T -C
```
```
ARG per_cpu_queues True
ARG peek_interval 30

Scheduler CPU 0 queue: ['a']
Scheduler CPU 1 queue: ['b']
Scheduler CPU 2 queue: ['c']
Scheduler CPU 3 queue: []
Scheduler CPU 4 queue: []

Finished time 55

Per-CPU stats
  CPU 0  utilization 100.00 [ warm 81.82 ]
  CPU 1  utilization 100.00 [ warm 81.82 ]
  CPU 2  utilization 100.00 [ warm 81.82 ]
  CPU 3  utilization 0.00 [ warm 0.00 ]
  CPU 4  utilization 0.00 [ warm 0.00 ]
```

9. Finally, feel free to just generate random workloads and see if you can predict their performance on different numbers of processors, cache sizes, and scheduling options. If you do this, you’ll soon be a multi-processor scheduling master, which is a pretty awesome thing to be. Good luck!
