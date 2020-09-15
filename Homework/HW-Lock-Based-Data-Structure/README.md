# Homework (Code)
In this homework, you’ll gain some experience with writing concurrent code and measuring its performance. Learning to build code that
performs well is a critical skill and thus gaining a little experience here
with it is quite worthwhile.

## Questions
1. We’ll start by redoing the measurements within this chapter. Use the call gettimeofday() to measure time within your program. How accurate is this timer? What is the smallest interval it can measure? Gain confidence in its workings, as we will need it in all subsequent questions. You can also look into other timers, such as the cycle counter available on x86 via the rdtsc instruction.

[gettimeofday(2) — Linux manual page](https://man7.org/linux/man-pages/man2/gettimeofday.2.html)

IMPORTANT NOTE:

" The time returned by gettimeofday() is affected by discontinuous
jumps in the system time (e.g., if the system administrator manually
changes the system time).  If you need a monotonically increasing
clock, see clock_gettime(2)."

gettimeofday() =>
- tv_sec : It is the number of seconds since the epoch.
- tv_usec :It is additional microseconds after number of seconds calculation since the epoch. .
```
struct    timeval  {
  time_t            tv_sec ;   //used for seconds
  suseconds_t       tv_usec ;   //used for microseconds
}
```

2. Now, build a simple concurrent counter and measure how long it takes to increment the counter many times as the number of threads increases. How many CPUs are available on the system you are using? Does this number impact your measurements at all?
```
// check CPU numbers
// Linux
$ cat /proc/cpuinfo
// FreeBSD, macOS
$ sysctl hw.ncpu
// macOS
$ sysctl hw.physicalcpu
hw.physicalcpu: 2
```
```
$ sysctl -n machdep.cpu.brand_string
Intel(R) Core(TM) i5-4278U CPU @ 2.60GHz
// I have 2 threads
$ sysctl -n machdep.cpu.core_count
2
// I have 4 threads
$ sysctl -n machdep.cpu.thread_count
4
$ gcc -pthread counter.c -o counter.out
$ ./counter.out

cpus is: 1
1 threads
global variable count: 1000000
Time (seconds): 0.054727

2 threads
global variable count: 2000000
Time (seconds): 0.152406

3 threads
global variable count: 3000000
Time (seconds): 0.185426

4 threads
global variable count: 4000000
Time (seconds): 0.236136

5 threads
global variable count: 5000000
Time (seconds): 0.299559
```
3. Next, build a version of the sloppy counter. Once again, measure its performance as the number of threads varies, as well as the threshold. Do the numbers match what you see in the chapter?
```
$ sysctl -n machdep.cpu.brand_string
Intel(R) Core(TM) i5-4278U CPU @ 2.60GHz

$ gcc -pthread sloppy_counter.c -o sloppy_counter.out
$ ./sloppy_counter.out

1 threads, 1 threshold
1000000 global counter
Time (seconds): 0.047459

2 threads, 1 threshold
2000000 global counter
Time (seconds): 0.169549

3 threads, 1 threshold
3000000 global counter
Time (seconds): 0.362046

4 threads, 1 threshold
4000000 global counter
Time (seconds): 0.458108

1 threads, 2 threshold
1000000 global counter
Time (seconds): 0.042697

2 threads, 2 threshold
2000000 global counter
Time (seconds): 0.107582

3 threads, 2 threshold
3000000 global counter
Time (seconds): 0.288652

4 threads, 2 threshold
4000000 global counter
Time (seconds): 0.385149
```

4. Build a version of a linked list that uses hand-over-hand locking [MS04], as cited in the chapter. You should read the paper first to understand how it works, and then implement it. Measure its performance. When does a hand-over-hand list work better than a standard list as shown in the chapter?

 the most popular approach to concurrent lockbased linked lists is hand-over-hand locking (sometimes called lock coupling) . In
this approach, each node has an associated lock. A thread traversing the linked list releases a node’s lock only after acquiring the lock of the next node in the list, thus preventing overtaking which may cause unnoticed removal of a node. This approach reduces lock granularity but significantly limits concurrency because insertions and deletions at disjoint list locations may delay each other.
```
$ gcc hand_over_hand_locking_list.c -o hand_over_hand_locking_list.out -pthreads
$ ./hand_over_hand_locking_list.out 2 1

1 threads
1
0
startusec: 1595692022903843
endusec 1595692022904058
Time (seconds): 0.000215

2 threads
1
0
1
0
startusec: 1595692022904118
endusec 1595692022904293
Time (seconds): 0.000175

3 threads
1
0
1
0
1
0
startusec: 1595692022904460
endusec 1595692022905373
Time (seconds): 0.000913

4 threads
1
0
1
0
1
0
1
0
startusec: 1595692022905413
endusec 1595692022907866
Time (seconds): 0.002453
```

5. Pick your favorite interesting data structure, such as a B-tree or other slightly more interested structure. Implement it, and start with a simple locking strategy such as a single lock. Measure its performance as the number of concurrent threads increases.

6. Finally, think of a more interesting locking strategy for this favorite data structure of yours. Implement it, and measure its performance. How does it compare to the straightforward locking approach?
