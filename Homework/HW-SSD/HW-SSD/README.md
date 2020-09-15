# Homework (Simulation)
This section introduces ssd.py, a simple SSD simulator you can use
to understand better how SSDs work. Read the README for details on
how to run the simulator. It is a long README, so boil a cup of tea (caffeinated likely necessary), put on your reading glasses, let the cat curl up on your lap1, and get to work.

```
$ ./ssd.py -h
```
```
Usage: ssd.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -n NUM_CMDS, --num_cmds=NUM_CMDS
                        number of commands to randomly generate
  -P OP_PERCENTAGES, --op_percentages=OP_PERCENTAGES
                        if rand, percent of reads/writes/trims
  -K SKEW, --skew=SKEW  if non-empty, skew, e.g., 80/20: 80% of ops to 20% of
                        blocks
  -k SKEW_START, --skew_start=SKEW_START
                        if --skew, skew after this many writes
  -r READ_FAIL, --read_fails=READ_FAIL
                        if rand, percent of reads that can fail
  -L CMD_LIST, --cmd_list=CMD_LIST
                        comma-separated list of commands (e.g., r10,w20:a)
  -T SSD_TYPE, --ssd_type=SSD_TYPE
                        SSD type: ideal, direct, log
  -l NUM_LOGICAL_PAGES, --logical_pages=NUM_LOGICAL_PAGES
                        number of logical pages in interface
  -B NUM_BLOCKS, --num_blocks=NUM_BLOCKS
                        number of physical blocks in SSD
  -p PAGES_PER_BLOCK, --pages_per_block=PAGES_PER_BLOCK
                        pages per physical block
  -G HIGH_WATER_MARK, --high_water_mark=HIGH_WATER_MARK
                        blocks used before gc trigger
  -g LOW_WATER_MARK, --low_water_mark=LOW_WATER_MARK
                        gc target before stopping gc
  -R READ_TIME, --read_time=READ_TIME
                        page read time (usecs)
  -W PROGRAM_TIME, --program_time=PROGRAM_TIME
                        page program time (usecs)
  -E ERASE_TIME, --erase_time=ERASE_TIME
                        page erase time (usecs)
  -J, --show_gc         show garbage collector behavior
  -F, --show_state      show flash state
  -C, --show_cmds       show commands
  -q, --quiz_cmds       quiz commands
  -S, --show_stats      show statistics
  -c, --compute         compute answers for me
```

## Questions

1. The homework will mostly focus on the log-structured SSD, which is simulated with the “-T log” flag. We’ll use the other types of SSDs for comparison. First, run with flags -T log -s 1 -n 10 -q. Can you figure out which operations took place? Use -c to check your answers (or just use -C instead of -q -c). Use different values of -s to generate different random workloads.
```
$ ./ssd.py -T log -s 1 -n 10 -q -C
```
write:
0:  3  26:  1  29:  4  49:  2  57:  0  54:  5
read:
9:: read(54) -> K
6:: read(57) -> u
trim:
29:  4 49:  2

Final state:
FTL     0:  3  26:  1  54:  5  57:  0

```
$ ./ssd.py -T log -s 2 -n 10 -q -C
```
```
$ ./ssd.py -T log -s 3 -n 10 -q -C
```

2. Now just show the commands and see if you can figure out the intermediate states of the Flash. Run with flags -T log -s 2 -n 10 -C to show each command. Now, determine the state of the Flash between each command; use -F to show the states and see if you were right. Use different random seeds to test your burgeoning expertise.
```
$ ./ssd.py -T log -s 2 -n 10 -C -F
```
```
$ ./ssd.py -T log -s 0 -n 10 -C -F
```
```
$ ./ssd.py -T log -s 1 -n 10 -C -F
```
3. Let’s make this problem ever so slightly more interesting by adding the -r 20 flag. What differences does this cause in the commands? Use -c again to check your answers.
```
$ ./ssd.py -T log -s 2 -n 10 -C -F -r 20
```
```
-r READ_FAIL, --read_fails=READ_FAIL
    if rand, percent of reads that can fail
```
It will create some percent failed read commands.

4. Performance is determined by the number of erases, programs, and reads (we assume here that trims are free). Run the same workload again as above, but without showing any intermediate states (e.g., -T log -s 1 -n 10). Can you estimate how long this workload will take to complete? (default erase time is 1000 microseconds, program time is 40, and read time is 10) Use the -S flag to check your answer. You can also change the erase, program, and read times with the -E, -W, -R flags.
```
$ ./ssd.py -T log -s 1 -n 10 -C -F -S
```
```
Logical Operation Sums
  Write count 6 (0 failed)
  Read count  2 (0 failed)
  Trim count  2 (0 failed)

Times
  Erase time 1000.00
  Write time 240.00
  Read time  20.00
  Total time 1260.00
```
5. Now, compare performance of the log-structured approach and the (very bad) direct approach (-T direct instead of -T log). First, estimate how you think the direct approach will perform, then check your answer with the -S flag. In general, how much better will the log-structured approach perform than the direct one?
```
$ ./ssd.py -T direct -s 1 -n 10 -C -F -S
```
```
Logical Operation Sums
  Write count 6 (0 failed)
  Read count  2 (0 failed)
  Trim count  2 (0 failed)

Times
  Erase time 6000.00
  Write time 320.00
  Read time  40.00
  Total time 6360.00
```
6. Let us next explore the behavior of the garbage collector. To do so, we have to set the high (-G) and low (-g) watermarks appropriately. First, let’s observe what happens when you run a larger workload to the log-structured SSD but without any garbage collection. To do this, run with flags -T log -n 1000 (the high watermark default is 10, so the GC won’t run in this configuration). What do you think will happen? Use -C and perhaps -F to see.
```
$ ./ssd.py -T log -n 1000 -C -J -G 120
```
The simulator is in a infinite loop.
Because the Disk is full of garbage we should activate some flag to know what is happening and avoid this behaviour.
Flags: 
```
 -J, --show_gc         show garbage collector behavior
 -G HIGH_WATER_MARK, --high_water_mark=HIGH_WATER_MARK
                       blocks used before gc trigger
```

7. To turn on the garbage collector, use lower values. The high watermark (-G N) tells the system to start collecting once N blocks have been used; the low watermark (-G M) tells the system to stop collecting once there are only M blocks in use. What watermark values do you think will make for a working system? Use -C and -F to show the commands and intermediate device states and see.
```
$ ./ssd.py -T log -n 1000 -C -J -G 120
```
The high watermark should be the page of each block.

8. One other useful flag is -J, which shows what the collector is doing when it runs. Run with flags -T log -n 1000 -C -J to see both the commands and the GC behavior. What do you notice about the GC? The final effect of GC, of course, is performance. Use -S to look at final statistics; how many extra reads and writes occur due to garbage collection? Compare this to the ideal SSD (-T ideal); how much extra reading, writing, and erasing is there due to the nature of Flash? Compare it also to the direct approach; in what way (erases, reads, programs) is the log-structured approach superior?
```
$ ./ssd.py -T log -s 1 -n 1000 -C -J -S -G 10 -g 8
```
```
Physical Operations Per Block
Erases  Sum: 93
Writes  Sum: 893
Reads   Sum: 822

Logical Operation Sums
  Write count 893 (0 failed)
  Read count  419 (0 failed)
  Trim count  91 (0 failed)

Times
  Erase time 93000.00
  Write time 35720.00
  Read time  8220.00
  Total time 136940.00
```
```
$ ./ssd.py -T ideal -n 1000 -C -J -S -G 10 -g 8
```
```
Physical Operations Per Block
Erases  Sum: 0
Writes  Sum: 506
Reads   Sum: 387

Logical Operation Sums
  Write count 506 (0 failed)
  Read count  387 (0 failed)
  Trim count  107 (0 failed)

Times
  Erase time 0.00
  Write time 20240.00
  Read time  3870.00
  Total time 24110.00
```
9. One last aspect to explore is workload skew. Adding skew to the workload changes writes such that more writes occur to some smaller fraction of the logical block space. For example, running with -K 80/20 makes 80% of the writes go to 20% of the blocks. Pick some different skews and perform many randomly-chosen operations (e.g., -n 1000), using first -T direct to understand the skew, and then-T log to see the impact on a log-structured device. What do you expect will happen? One other small skew control to explore is -k 100; by adding this flag to a skewed workload, the first 100 writes are not skewed. The idea is to first create a lot of data, but then only update some of it. What impact might that have upon a garbage collector?
```
$ ./ssd.py -T direct -s 1 -n 1000 -G 10 -g 8 -S -K 80/20
```
```
Erases Sum: 497
Writes Sum: 4046
Reads  Sum: 4378

Logical Operation Sums
  Write count 497 (0 failed)
  Read count  397 (0 failed)
  Trim count  106 (0 failed)

Times
  Erase time 497000.00
  Write time 161840.00
  Read time  43780.00
  Total time 702620.00
```
```
$ ./ssd.py -T log -s 1 -n 1000 -G 10 -g 8 -S -K 80/20 -J -C
```
```
Physical Operations Per Block
Erases   Sum: 60
Writes   Sum: 568
Reads    Sum: 468

Logical Operation Sums
  Write count 568 (0 failed)
  Read count  397 (0 failed)
  Trim count  106 (0 failed)

Times
  Erase time 60000.00
  Write time 22720.00
  Read time  4680.00
  Total time 87400.00
```
```
$ ./ssd.py -T direct -s 1 -n 1000 -G 10 -g 8 -S -K 80/20 -k 100
```
```
Physical Operations Per Block
Erases Sum: 493
Writes Sum: 4233
Reads  Sum: 4562

Logical Operation Sums
  Write count 493 (0 failed)
  Read count  405 (0 failed)
  Trim count  102 (0 failed)

Times
  Erase time 493000.00
  Write time 169320.00
  Read time  45620.00
  Total time 707940.00
```
```
$ ./ssd.py -T log -s 1 -n 1000 -G 10 -g 8 -S -K 80/20 -k 100 -J -C
```
```
Physical Operations Per Block
Erases  Sum: 66
Writes  Sum: 624
Reads   Sum: 536

Logical Operation Sums
  Write count 624 (0 failed)
  Read count  405 (0 failed)
  Trim count  102 (0 failed)

Times
  Erase time 66000.00
  Write time 24960.00
  Read time  5360.00
  Total time 96320.00
```
