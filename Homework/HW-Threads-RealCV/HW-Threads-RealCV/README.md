# Homework (Code)
This homework lets you explore some real code that uses locks and
condition variables to implement various forms of the producer/consumer
queue discussed in the chapter. You’ll look at the real code, run it in
various configurations, and use it to learn about what works and what
doesn’t, as well as other intricacies. Read the README for details.

```
-l <number of items each producer produces>
-m <size of the shared producer/consumer buffer>
-p <number of producers>
-c <number of consumers>
-P <sleep string: how producer should sleep at various points>
-C <sleep string: how consumer should sleep at various points>
-v [verbose flag: trace what is happening and print it]
-t [timing flag: time entire execution and print total time]
```

## Questions
1. Our first question focuses on main-two-cvs-while.c (the working solution). First, study the code. Do you think you have an understanding of what should happen when you run the program?

The main thread starts.
p producer -> produce threads and c consumer -> consume threads.
Each producer places 1 item in a buffer with a capacity of m and then exit. The main thread waits for all producers to exit, and then adds c EOF marks to the buffer.
The consumer thread automatically exits after seeing the EOF mark and returns how many items have been consumed.
The main thread finally waits for all consumer threads to exit and prints statistics.

2. Run with one producer and one consumer, and have the producer produce a few values. Start with a buffer (size 1), and then increase it. How does the behavior of the code change with larger buffers? (or does it?) What would you predict num full to be with different buffer sizes (e.g., -m 10) and different numbers of produced items (e.g., -l 100), when you change the consumer sleep string from default (no sleep) to -C 0,0,0,0,0,0,1?

```
$ ./main-two-cvs-while -l 5 -m 1 -p 1 -c 1 -v
```
```
$ ./main-two-cvs-while -l 5 -m 2 -p 1 -c 1 -v
```
```
$ ./main-two-cvs-while -l 10 -m 2 -p 1 -c 1 -v
```
```
$ ./main-two-cvs-while -l 10 -m 10 -p 1 -c 1 -v
```
```
$ ./main-two-cvs-while -l 100 -m 10 -p 1 -c 1 -v
```
```
$ ./main-two-cvs-while -l 100 -m 10 -p 1 -c 1 -C 0,0,0,0,0,0,1 -v 
```

The larger the buffer the earlier the producer thread exits.
-C 0,0,0,0,0,0,1 means that the consumer thread sleeps for 1 second after taking the data and release the lock. In this 1s time, enough producers can fill up the buffer. 

3. If possible, run the code on different systems (e.g., a Mac and Linux). Do you see different behavior across these systems?

4. Let’s look at some timings. How long do you think the following execution, with one producer, three consumers, a single-entry shared buffer, and each consumer pausing at point c3 for a second, will take? ./main-two-cvs-while -p 1 -c 3 -m 1 -C 0,0,0,1,0,0,0:0,0,0,1,0,0,0:0,0,0,1,0,0,0 -l 10 -v -t
```
$ ./main-two-cvs-while -p 1 -c 3 -m 1 -C 0,0,0,1,0,0,0:0,0,0,1,0,0,0:0,0,0,1,0,0,0 -l 10 -v -t

Consumer consumption:
  C0 -> 6
  C1 -> 2
  C2 -> 2

Total time: 12.04 seconds
```
To know the total time running time we need to know how many times the consumer will be awakened.
the proportion of consumer consumption as we can see above is greater for one of the consumer than the others because the others are sleeping when the producer produce threads and only one of them are awake to consume it.

5. Now change the size of the shared buffer to 3 (-m 3). Will this make any difference in the total time?
```
$ ./main-two-cvs-while -p 1 -c 3 -m 3 -C 0,0,0,1,0,0,0:0,0,0,1,0,0,0:0,0,0,1,0,0,0 -l 10 -v -t

Consumer consumption:
  C0 -> 5
  C1 -> 3
  C2 -> 2

Total time: 5.02 seconds
```
The total time is half since all the consumer have more probability to stay awake and consume threads because the producer should put 3 at once, and the consumer consumes 3 at a time. So there are more proportion in the consumer consumption.
Every time the producer puts one into the buffer, the constext switches to a consumer that is not sleeping, this consumes once and then happens to switch to another consumer awakened by the producer then loses 1s, and then schedule the producer to put an item to switch context.

6. Now change the location of the sleep to c6 (this models a consumer taking something off the queue and then doing something with it), again using a single-entry buffer. What time do you predict in this case? ./main-two-cvs-while -p 1 -c 3 -m 1 -C 0,0,0,0,0,0,1:0,0,0,0,0,0,1:0,0,0,0,0,0,1 -l 10 -v -t

```
$ ./main-two-cvs-while -p 1 -c 3 -m 1 -C 0,0,0,0,0,0,1:0,0,0,0,0,0,1:0,0,0,0,0,0,1 -l 10 -v -t

Consumer consumption:
  C0 -> 4
  C1 -> 3
  C2 -> 3

Total time: 5.03 seconds
```
It call sleep 12 times, but only uses 5 seconds. That is because sleep after releasing the lock so other thread can move on and get data from the buffer.

7. Finally, change the buffer size to 3 again (-m 3). What time do you predict now?

```
$ ./main-two-cvs-while -p 1 -c 3 -m 3 -C 0,0,0,0,0,0,1:0,0,0,0,0,0,1:0,0,0,0,0,0,1 -l 10 -v -t

Consumer consumption:
  C0 -> 3
  C1 -> 4
  C2 -> 3

Total time: 5.02 seconds
```
The consumption rate is 3 per second. The size of the buffer does not affect the processing speed. So the Total time is same as the question 6.
It call sleep 13 times.

8. Now let’s look at main-one-cv-while.c. Can you configure a sleep string, assuming a single producer, one consumer, and a buffer of size 1, to cause a problem with this code?
```
$ ./main-one-cvs-while -p 1 -c 1 -m 1 -v
```
No.

9. Now change the number of consumers to two. Can you construct sleep strings for the producer and the consumers so as to cause a problem in the code?
```
$ ./main-one-cvs-while -p 1 -c 2 -m 1 -v -t -P 1
```
the consumer exit after consiming.

10. Now examine main-two-cvs-if.c. Can you cause a problem to happen in this code? Again consider the case where there is only one consumer, and then the case where there is more than one.
```
$ ./main-two-cvs-if -p 1 -c 1 -m 1 -v -t
```
```
$ ./main-two-cvs-if -p 1 -c 2 -m 1 -v -t
```
This example is like Figure 30.8
Two critical problems:
- Another consumer sneaks in and consumes the one existing value in the buffer. then the first consumer runs; just before returning from the wait but there are no buffers to consume. The state of the world has changed, but there is no guarantee that when the woken thread runs, the state will still be as desired.

Thanks to Mesa semantics, a simple rule to remember with condition
variables is to always use while loops.

- The second problem: there is only one condition variable.
The problem occurs when two consumers run first and both go to sleep. Then, the producer runs, put a value in the buffer, and wakes one of the consumers this consume the value. Which thread should it wake ?
In this case since the buffer is empty the producer should wake up to fill the buffer but with only general cv this is not the case. When the consumer wake up instead all three threads are left sleeping.

11. Finally, examine main-two-cvs-while-extra-unlock.c. What problem arises when you release the lock before doing a put or a get? Can you reliably cause such a problem to happen, given the sleep strings? What bad thing can happen?

```
$ $ ./main-two-cvs-while-extra-unlock -p 1 -c 2 -m 10 -l 10 -v -C 0,0,0,0,1,0,0:0,0,0,0,0,0,0

Consumer consumption:
C0 -> 1
C1 -> 9
```
one of the consumers just consume one value since is sleeping before doing do_get( ).
