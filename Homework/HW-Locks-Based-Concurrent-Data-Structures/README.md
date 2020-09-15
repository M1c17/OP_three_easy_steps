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
// Runs on 2.6 GHz Dual-Core Intel Core i5
$ gcc -pthread counter.c -o counter.out
$ ./counter.out


```
3. Next, build a version of the sloppy counter. Once again, measure its performance as the number of threads varies, as well as the threshold. Do the numbers match what you see in the chapter?
4. Build a version of a linked list that uses hand-over-hand locking [MS04], as cited in the chapter. You should read the paper first to understand how it works, and then implement it. Measure its performance. When does a hand-over-hand list work better than a standard list as shown in the chapter?
5. Pick your favorite interesting data structure, such as a B-tree or other slightly more interested structure. Implement it, and start with a simple locking strategy such as a single lock. Measure its performance as the number of concurrent threads increases.
6. Finally, think of a more interesting locking strategy for this favorite data structure of yours. Implement it, and measure its performance. How does it compare to the straightforward locking approach?
