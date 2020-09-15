# Homework (Simulation)
This program, lottery.py, allows you to see how a lottery scheduler works. See the README for details.

But first:

```
$ ./lottery.py -h
```
```
Usage: lottery.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -j JOBS, --jobs=JOBS  number of jobs in the system
  -l JLIST, --jlist=JLIST
                        instead of random jobs, provide a comma-separated list
                        of run times and ticket values (e.g., 10:100,20:100
                        would have two jobs with run-times of 10 and 20, each
                        with 100 tickets)
  -m MAXLEN, --maxlen=MAXLEN
                        max length of job
  -T MAXTICKET, --maxticket=MAXTICKET
                        maximum ticket value, if randomly assigned
  -q QUANTUM, --quantum=QUANTUM
                        length of time slice
  -c, --compute         compute answers for me
```

## Questions

1. Compute the solutions for simulations with 3 jobs and random seeds of 1, 2, and 3.

```
$ ./lottery.py -s 1 -j 3
```
```
ARG jlist 
ARG jobs 3
ARG maxlen 10
ARG maxticket 100
ARG quantum 1
ARG seed 1

Here is the job list, with the run time of each job: 
  Job 0 ( length = 1, tickets = 84 )
  Job 1 ( length = 7, tickets = 25 )
  Job 2 ( length = 4, tickets = 44 )


Here is the set of random numbers you will need (at most):
Random 651593
Random 788724
Random 93859
Random 28347
Random 835765
Random 432767
Random 762280
Random 2106
Random 445387
Random 721540
Random 228762
Random 945271
```
```
$ ./lottery.py -s 2 -j 3
```
```
ARG jlist 
ARG jobs 3
ARG maxlen 10
ARG maxticket 100
ARG quantum 1
ARG seed 2

Here is the job list, with the run time of each job: 
  Job 0 ( length = 9, tickets = 94 )
  Job 1 ( length = 8, tickets = 73 )
  Job 2 ( length = 6, tickets = 30 )


Here is the set of random numbers you will need (at most):
Random 605944
Random 606802
Random 581204
Random 158383
Random 430670
Random 393532
Random 723012
Random 994820
Random 949396
Random 544177
Random 444854
Random 268241
Random 35924
Random 27444
Random 464894
Random 318465
Random 380015
Random 891790
Random 525753
Random 560510
Random 236123
Random 23858
Random 325143
```
```
$ ./lottery.py -s 3 -j 3
```
```
ARG jlist 
ARG jobs 3
ARG maxlen 10
ARG maxticket 100
ARG quantum 1
ARG seed 3

Here is the job list, with the run time of each job: 
  Job 0 ( length = 2, tickets = 54 )
  Job 1 ( length = 3, tickets = 60 )
  Job 2 ( length = 6, tickets = 6 )


Here is the set of random numbers you will need (at most):
Random 13168
Random 837469
Random 259354
Random 234331
Random 995645
Random 470263
Random 836462
Random 476353
Random 639068
Random 150616
Random 634861
```

2. Now run with two specific jobs: each of length 10, but one (job 0) with just 1 ticket and the other (job 1) with 100 (e.g., -l 10:1,10:100). What happens when the number of tickets is so imbalanced? Will job 0 ever run before job 1 completes? How often? In general, what does such a ticket imbalance do to the behavior of lottery scheduling?

```
$ ./lottery.py -l 10:1,10:100 -c
```
The job with most tickets will surely monoplize the cpu. The chances that job 0 ever run before job 1 is just less than 1% (0.99099%). Such ticket imbalance change the behavior of lottery scheduler from fair-share schedular to unfair-share schedular. It would look similar to sequential schedular which run jobs in some pre-defined order.

3. When running with two jobs of length 100 and equal ticket allocations of 100 (-l 100:100,100:100), how unfair is the scheduler? Run with some different random seeds to determine the (probabilistic) answer; let unfairness be determined by how much earlier one job finishes than the other.

```
$ ./lottery.py -s 0 -l 100:100,100:100 -c
```
```
--> JOB 0 DONE at time 192
--> JOB 1 DONE at time 200
```
Unfairness Metric = 192/200 = 0.96
```
$ ./lottery.py -s 1 -l 100:100,100:100 -c
```
```
--> JOB 1 DONE at time 196
--> JOB 0 DONE at time 200
```
Unfairness Metric = 196/200 = 0.98
```
$ ./lottery.py -s 2 -l 100:100,100:100 -c
```
```
--> JOB 1 DONE at time 190
--> JOB 0 DONE at time 200
```
Unfairness Metric = 190/200 = 0.95
```
$ ./lottery.py -s 3 -l 100:100,100:100 -c
```
```
--> JOB 0 DONE at time 196
--> JOB 1 DONE at time 200
```
Unfairness Metric = 196/200 = 0.98

4. How does your answer to the previous question change as the quantum size (-q) gets larger?

| q-size | Unfairness |
| :---: | :---: |
| 1 | 0.98 |
| 2 | 0.97 |
| 3 | 0.985 |
| 4 | 0.94 |
| 5 | 0.95 |
| 6 | 0.941 |
| 7 | 0.866 |
| 8 | 0.8846 |
| 9 | 0.8333 |
| 10 | 0.8 |

You can see in the table above with the increase of quantum size the unfairness grows.

5. Can you make a version of the graph that is found in the chapter? What else would be worth exploring? How would the graph look with a stride scheduler?

