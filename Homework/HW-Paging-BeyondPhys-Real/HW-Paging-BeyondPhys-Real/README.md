# Homework (Measurement)
This homework introduces you to a new tool, vmstat, and how it can
be used to understand memory, CPU, and I/O usage. Read the associated README and examine the code in mem.c before proceeding to the exercises and questions below.
 
## Questions

1. First, open two separate terminal connections to the same machine, so that you can easily run something in one window and the other. Now, in one window, run vmstat 1, which shows statistics about machine usage every second. Read the man page, the associated README, and any other information you need so that you can understand its output. Leave this window running vmstat for the rest of the exercises below. Now, we will run the program mem.c but with very little memory usage. This can be accomplished by typing ./mem 1 (which uses only 1 MB of memory).
```
$ ./mem 1
```

How do the CPU usage statistics change when running mem?
The user time increase and the idle time decrease.

Do the numbers in the user time column make sense?
Yes. you can notice the increment in the activity of other process running.
you can see also how pageouts are going up (which would mean that the machine has to page memory out on disk to make room for the new ones).

How does this change when running more than one instance of mem at once?
```
$ ./mem 1 & ./mem 2
```
the user time will double size.

2. Let’s now start looking at some of the memory statistics while running mem. We’ll focus on two columns: swpd (the amount of virtual memory used) and free (the amount of idle memory). Run ./mem 1024 (which allocates 1024 MB) and watch how these values change. Then kill the running program (by typing control-c) and watch again how the values change. What do you notice about the values? In particular, how does the free column change when the program exits? 
```
$ ./mem 1024
```
In this case the amount of idle memory go up almost 3 times and for the case of the amount of virtual memory used was the same while the program was running until I kill the program the amounts goes down.

Does the amount of free memory increase by the expected amount when mem exits?
Yes.

3. We’ll next look at the swap columns (si and so), which indicate how much swapping is taking place to and from the disk. Of course, to activate these, you’ll need to run mem with large amounts of memory. First, examine how much free memory is on your Linux system (for example, by typing cat /proc/meminfo; type man proc for details on the /proc file system and the types of information you can find there). One of the first entries in /proc/meminfo is the total amount of memory in your system. Let’s assume it’s something like 8 GB of memory; if so, start by running mem 4000 (about 4 GB) and watching the swap in/out columns. Do they ever give non-zero values? Then, try with 5000, 6000, etc. What happens to these values as the program enters the second loop (and beyond), as compared to the first loop? How much data (total) are swapped in and out during the second, third, and subsequent loops? (do the numbers make sense?)
```
$ ./mem 4000
```
```
$ ./mem 5000
```
```
$ ./mem 6000
```
When run for the first time with 4000 the swap columns change from 0 going up specially at the beginning swapouts; As the size of GB increase in my input 5000 and 6000 the second and third loops increase as well. when I kill the program the colum of swapoins still was increase and the column of swapouts start to goes down until become 0.


4. Do the same experiments as above, but now watch the other statistics (such as CPU utilization, and block I/O statistics). How do they change when mem is running?

User time and the other statistics (such as CPU utilization, and block I/O statistics) both increase  as well.

5. Now let’s examine performance. Pick an input for mem that comfortably fits in memory (say 4000 if the amount of memory on the system is 8 GB). How long does loop 0 take (and subsequent loops 1, 2, etc.)? Now pick a size comfortably beyond the size of memory (say 12000 again assuming 8 GB of memory). How long do the loops take here? How do the bandwidth numbers compare? How different is performance when constantly swapping versus fitting everything comfortably in memory? Can you make a graph, with the size of memory used by mem on the x-axis, and the bandwidth of accessing said memory on the y-axis? Finally, how does the performance of the first loop compare to that of subsequent loops, for both the case where everything fits in memory and where it doesn’t?
```
$ ./mem 2000
```
```
allocating 2097152000 bytes (2000.00 MB)
  number of integers in array: 524288000
loop 0 in 1495.72 ms (bandwidth: 1337.15 MB/s)
loop 1 in 552.72 ms (bandwidth: 3618.45 MB/s)
loop 2 in 447.61 ms (bandwidth: 4468.22 MB/s)
loop 3 in 458.51 ms (bandwidth: 4361.92 MB/s)
loop 4 in 587.40 ms (bandwidth: 3404.81 MB/s)
loop 5 in 441.47 ms (bandwidth: 4530.37 MB/s)
loop 6 in 452.83 ms (bandwidth: 4416.68 MB/s)
loop 7 in 467.23 ms (bandwidth: 4280.55 MB/s)
loop 8 in 458.88 ms (bandwidth: 4358.45 MB/s)
loop 9 in 439.29 ms (bandwidth: 4552.85 MB/s)
loop 10 in 415.14 ms (bandwidth: 4817.65 MB/s)
```
```
$ ./mem 4000
```
```
allocating 4194304000 bytes (4000.00 MB)
  number of integers in array: 1048576000
loop 0 in 4066.80 ms (bandwidth: 983.58 MB/s)
loop 1 in 4951.17 ms (bandwidth: 807.89 MB/s)
loop 2 in 4929.70 ms (bandwidth: 811.41 MB/s)
loop 3 in 4874.72 ms (bandwidth: 820.56 MB/s)
loop 4 in 4426.68 ms (bandwidth: 903.61 MB/s)
loop 5 in 4063.30 ms (bandwidth: 984.42 MB/s)
loop 6 in 3525.85 ms (bandwidth: 1134.48 MB/s)
loop 7 in 3176.90 ms (bandwidth: 1259.09 MB/s)
```
```
$ ./mem 12000
```
```
allocating 12582912000 bytes (12000.00 MB)
  number of integers in array: 3145728000
Segmentation fault: 11
```


6. Swap space isn’t infinite. You can use the tool swapon with the -s flag to see how much swap space is available. What happens if you try to run mem with increasingly large values, beyond what seems to be available in swap? At what point does the memory allocation fail?



7. Finally, if you’re advanced, you can configure your system to use different swap devices using swapon and swapoff. Read the man pages for details. If you have access to different hardware, see how the performance of swapping changes when swapping to a classic hard drive, a flash-based SSD, and even a RAID array. How much can swapping performance be improved via newer devices? How close can you get to in-memory performance?
