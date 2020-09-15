# Homework (Code)
This homework lets you explore some real code that deadlocks (or
avoids deadlock). The different versions of code correspond to different
approaches to avoiding deadlock in a simplified vector add() routine.
See the README for details on these programs and their common substrate.
Each program takes the same set of arguments (see main-common.c for details):
```
-d                 
   This flag turns on the ability for threads to deadlock.
   When you pass -d to the program, every other thread calls vector_add()
   with the vectors in a different order, e.g., with two threads, and -d enabled, 
   Thread 0 calls vector_add(v1, v2) and Thread 1 calls vector_add(v2, v1)

-p
   This flag gives each thread a different set of vectors to call add()
   upon, instead of just two vectors. Use this to see how things perform
   when there isn't contention for the same set of vectors.

-n num_threads
   Creates some number of threads; you need more than one to deadlock.

-l loops
   How many times should each thread call vector_add()?

-v 
   Verbose flag: prints out a little more about what is going on.

-t
   Turns on timing and shows how long everything took.
```
```
typedef struct __thread_arg_t {
    int tid;                     // thread id
    int vector_add_order;        // call order, 0 means call add(v[v0], v[v1]), 1 means call add(v[v1], v[v0])
    int vector_0;                // In which vector the call occurred, the index of the v array v0
    int vector_1;                // In which vector the call occurred, the index of the v array v1
} thread_arg_t;
```

## Questions
1. First let’s make sure you understand how the programs generally work, and some of the key options. Study the code in vector-deadlock.c, as well as in main-common.c and related files. Now, run ./vector-deadlock -n 2 -l 1 -v, which instantiates two threads (-n 2), each of which does one vector add (-l 1), and does so in verbose mode (-v). Make sure you understand the output. How does the output change from run to run?
```
$ ./vector-deadlock -n 2 -l 1 -v

->add(0, 1)
              ->add(0, 1)
              <-add(0, 1)
<-add(0, 1)

$ ./vector-deadlock -n 2 -l 1 -v

->add(0, 1)
<-add(0, 1)
              ->add(0, 1)
              <-add(0, 1)
```
thwo threads dont cause deadlock.
Theorically the execution order of thread 1 and thread 2 is not fixed.

2. Now add the -d flag, and change the number of loops (-l) from 1 to higher numbers. What happens? Does the code (always) deadlock?

```
$ ./vector-deadlock -n 2 -l 1000000 -d -v
```
It does not always deadlock but sometimes. 

3. How does changing the number of threads (-n) change the outcome of the program? Are there any values of -n that ensure no deadlock occurs?
```
$ ./vector-deadlock -n 4 -l 10 -v
```
```
$ ./vector-deadlock -n 3 -l 10 -v
```
```
$ ./vector-deadlock -n 6 -l 10 -v
```
-n number of threads increase the probability of interleaving.

4. Now examine the code in vector-global-order.c. First, make sure you understand what the code is trying to do; do you understand why the code avoids deadlock? Also, why is there a special case in this vector add() routine when the source and destination vectors are the same?

Using the size of the pointer to determine ordering. So you avoid deadlock.
Calling the same array will produce an equal situation.

5. Now run the code with the following flags: -t -n 2 -l 100000 -d. How long does the code take to complete? How does the total time change when you increase the number of loops, or the number of threads?
```
$ ./vector-global-order -t -n 2 -l 100000 -d
Time: 0.02 seconds
```
Increasing -l
```
$ ./vector-global-order -t -n 2 -l 1000000 -d
Time: 0.19 seconds
```
Increading -n
```
$ ./vector-global-order -t -n 4 -l 100000 -d
Time: 0.20 seconds
```
```
$ ./vector-global-order -t -n 6 -l 1000000 -d
Time: 1.40 seconds
```
```
$ ./vector-global-order -t -n 2 -l 1000000 -d
Time: 0.25 seconds
```
As the -n number of thread increase the time also increase since there are more interleaving between threads
As the number of loops increase the time also increase since increase the cycle time.

6. What happens if you turn on the parallelism flag (-p)? How much would you expect performance to change when each thread is working on adding different vectors (which is what -p enables) versus working on the same ones?
```
$ ./vector-global-order -t -n 2 -l 100000 -p -d
Time: 0.01 seconds
```
```
$ ./vector-global-order -t -n 2 -l 700000 -p -d
Time: 0.07 seconds
```
```
$ ./vector-global-order -t -n 4 -l 500000 -p -d
Time: 0.09 seconds
```
```
./vector-global-order -t -n 6 -l 1000000 -d -p
Time: 0.25 seconds
```
We can see when working with parallelism the time decrease significantly
the locking between each thread doesnot conflict, and there is no need to wait for the execution of others threads.
Multi-core CPU can be fully utilized, with higher concurrency.

7. Now let’s study vector-try-wait.c. First make sure you understand the code. Is the first call to pthread mutex trylock() really needed? Now run the code. How fast does it run compared to the global order approach? How does the number of retries, as counted by the code, change as the number of threads increases?

```
$ ./vector-try-wait -t -n 2 -l 100000 -d
Retries: 1479119
Time: 0.12 seconds
```
```
$ ./vector-try-wait -t -n 6 -l 1000000 -d
livelock.....
```
```
$ ./vector-try-wait -t -n 2 -l 1000000 -d
Retries: 14049359
Time: 0.99 seconds
$ ./vector-global-order -t -n 2 -l 1000000 -d
Time: 0.19 seconds
```

No Preemption ->"Such an interface could be used as follows to build a deadlock-free, ordering-robust lock acquisition protocol."

this is slower than global order approach without parallelism.
Must be careful with this approach since in the second case there are a livelock happening.

8. Now let’s look at vector-avoid-hold-and-wait.c. What is the main problem with this approach? How does its performance compare to the other versions, when running both with -p and without it?

```
$ ./vector-avoid-hold-and-wait -t -n 2 -l 10000 -d -v
Time: 4.75 seconds
```
```
$ ./vector-avoid-hold-and-wait -t -n 2 -l 10000 -d -p -v
Time: 4.66 seconds
```
Since this approach have a global lock that dont leave others thread adquire the local lock acquisition the time is bigger.

The degree of concurrency is basic, and even when we enable -p is also lomited by the global lock.

9. Finally, let’s look at vector-nolock.c. This version doesn’t use locks at all; does it provide the exact same semantics as the other versions? Why or why not?
```
$ ./vector-nolock -t -n 2 -l 10000 -d -v
Time: 2.64 seconds
```
This approach is using hardware instructions fetch-and-add in orden to be lockfree.

"In this manner, no lock is acquired, and no deadlock can arise (though livelock is still a possibility, and thus a robust solution will be more complex than this simple code)."

10. Now compare its performance to the other versions, both when threads are working on the same two vectors (no -p) and when each thread is working on separate vectors (-p). How does this no-lock version perform?
```
$ ./vector-nolock -t -n 2 -l 10000 -d -v
Time: 2.50 seconds
```
```
$ ./vector-nolock -t -n 2 -l 10000 -d -v -p
Time: 2.24 seconds
```
The hardware instructions wastes a lot of time cycles.
