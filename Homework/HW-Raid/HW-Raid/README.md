# Homework (Simulation)
This section introduces raid.py, a simple RAID simulator you can
use to shore up your knowledge of how RAID systems work. See the README for details.

```
Usage: raid.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -D NUMDISKS, --numDisks=NUMDISKS
                        number of disks in RAID
  -C CHUNKSIZE, --chunkSize=CHUNKSIZE
                        chunk size of the RAID
  -n NUMREQUESTS, --numRequests=NUMREQUESTS
                        number of requests to simulate
  -S SIZE, --reqSize=SIZE
                        size of requests
  -W WORKLOAD, --workload=WORKLOAD
                        either "rand" or "seq" workloads
  -w WRITEFRAC, --writeFrac=WRITEFRAC
                        write fraction (100->all writes, 0->all reads)
  -R RANGE, --randRange=RANGE
                        range of requests (when using "rand" workload)
  -L LEVEL, --level=LEVEL
                        RAID level (0, 1, 4, 5)
  -5 RAID5TYPE, --raid5=RAID5TYPE
                        RAID-5 left-symmetric "LS" or left-asym "LA"
  -r, --reverse         instead of showing logical ops, show physical
  -t, --timing          use timing mode, instead of mapping mode
  -c, --compute         compute answers for me
```

## Questions
1. Use the simulator to perform some basic RAID mapping tests. Run with different levels (0, 1, 4, 5) and see if you can figure out the mappings of a set of requests. For RAID-5, see if you can figure out the difference between left-symmetric and left-asymmetric layouts. Use some different random seeds to generate different problems than above.
```
$ ./raid.py -n 5 -R 20 -L 0 -s 5 -c

```
```
$ ./raid.py -n 5 -R 20 -L 1 -s 4 -c 

```
```
$ ./raid.py -n 5 -R 20 -L 4 -s 0 -c

```
```
$ ./raid.py -n 5 -R 20 -L 5 -s 2 -5 LS -c -W seq
$ ./raid.py -n 5 -R 20 -L 5 -s 2 -5 LA -c -W seq

left-symmetric    left-asymmetric
0 1 2 P           0 1 2 P
4 5 P 3           3 4 P 5
8 P 6 7           6 P 7 8
```
" With RAID-5, two different layout schemes are available, left-symmetric and left-asymmetric; use -5 LS or -5 LA to try those out with RAID-5 (-L 5)."


2. Do the same as the first problem, but this time vary the chunk size with -C. How does chunk size change the mappings?
```
$ ./raid.py -n 5 -R 20 -L 0 -s 5 -c -C 8K

0  2  4  P
1  3  5  P
8 10  P  6
9 11  P  7
12 P 14
13 P 15
```
```
$ ./raid.py -n 15 -R 20 -L 5 -5 LS -c -W seq -C 8K
```
3. Do the same as above, but use the -r flag to reverse the nature of each problem.

```
$ ./raid.py -n 15 -R 20 -L 5 -5 LS -c -W seq -C 8K -r
```

4. Now use the reverse flag but increase the size of each request with the -S flag. Try specifying sizes of 8k, 12k, and 16k, while varying the RAID level. What happens to the underlying I/O pattern when  the size of the request increases? Make sure to try this with the sequential workload too (-W sequential); for what request sizes are RAID-4 and RAID-5 much more I/O efficient?

```
$ ./raid.py -L 4 -S 4k -c -W seq
$ ./raid.py -L 4 -S 8k -c -W seq
$ ./raid.py -L 4 -S 12k -c -W seq
$ ./raid.py -L 4 -S 16k -c -W seq
$ ./raid.py -L 5 -S 4k -c -W seq
$ ./raid.py -L 5 -S 8k -c -W seq
$ ./raid.py -L 5 -S 12k -c -W seq
$ ./raid.py -L 5 -S 16k -c -W seq
```
RAID-4 and RAID-5 are more efficient with 16k size of request

5. Use the timing mode of the simulator (-t) to estimate the performance of 100 random reads to the RAID, while varying the RAID levels, using 4 disks.
```
$ ./raid.py -L 0 -n 100 -t -c

disk:0  busy: 100.00  I/Os:    28 (sequential:0 nearly:1 random:27)
disk:1  busy:  93.91  I/Os:    29 (sequential:0 nearly:6 random:23)
disk:2  busy:  87.92  I/Os:    24 (sequential:0 nearly:0 random:24)
disk:3  busy:  65.94  I/Os:    19 (sequential:0 nearly:1 random:18)

STAT totalTime 275.69999999999993

$ ./raid.py -L 1 -n 100 -t -c

disk:0  busy:  58.85  I/Os:    28 (sequential:0 nearly:1 random:27)
disk:1  busy:  51.18  I/Os:    24 (sequential:0 nearly:0 random:24)
disk:2  busy:   0.00  I/Os:     0 (sequential:0 nearly:0 random:0)
disk:3  busy: 100.00  I/Os:    48 (sequential:0 nearly:2 random:46)

STAT totalTime 473.6000000000004

$ ./raid.py -L 4 -n 100 -t -c

disk:0  busy:  78.48  I/Os:    30 (sequential:0 nearly:0 random:30)
disk:1  busy: 100.00  I/Os:    40 (sequential:0 nearly:3 random:37)
disk:2  busy:  76.46  I/Os:    30 (sequential:0 nearly:2 random:28)
disk:3  busy:   0.00  I/Os:     0 (sequential:0 nearly:0 random:0)

STAT totalTime 386.1000000000002

$ ./raid.py -L 5 -n 100 -t -c

disk:0  busy: 100.00  I/Os:    28 (sequential:0 nearly:1 random:27)
disk:1  busy:  95.84  I/Os:    29 (sequential:0 nearly:5 random:24)
disk:2  busy:  87.60  I/Os:    24 (sequential:0 nearly:0 random:24)
disk:3  busy:  65.70  I/Os:    19 (sequential:0 nearly:1 random:18)

STAT totalTime 276.7
```

6. Do the same as above, but increase the number of disks. How does the performance of each RAID level scale as the number of disks increases?
```
$ ./raid.py -L 0 -n 100 -D 4 -t -c
STAT totalTime 275.69999999999993

$ ./raid.py -L 1 -n 100 -D 4 -t -c
STAT totalTime 473.6000000000004

$ ./raid.py -L 4 -n 100 -D 4 -t -c
STAT totalTime 386.1000000000002

$ ./raid.py -L 5 -n 100 -D 4 -t -c
STAT totalTime 276.7
```
```
$ ./raid.py -L 0 -n 100 -D 8 -t -c
STAT totalTime 156.49999999999994

$ ./raid.py -L 1 -n 100 -D 8 -t -c
STAT totalTime 258.8999999999999

$ ./raid.py -L 4 -n 100 -D 8 -t -c
STAT totalTime 164.99999999999994

$ ./raid.py -L 5 -n 100 -D 8 -t -c
STAT totalTime 158.59999999999997
```
As we can see above as the number of disks increase the total time decrease so better performance.

7. Do the same as above, but use all writes (-w 100) instead of reads. How does the performance of each RAID level scale now? Can you do a rough estimate of the time it will take to complete the workload of 100 random writes?

```
$ ./raid.py -L 0 -n 100 -D 4 -w 100 seq -t -c
STAT totalTime 275.69999999999993

$ ./raid.py -L 1 -n 100 -D 4 -w 100 seq -t -c
STAT totalTime 509.80000000000047

$ ./raid.py -L 4 -n 100 -D 4 -w 100 seq -t -c
STAT totalTime 982.4666666666681

$ ./raid.py -L 5 -n 100 -D 4 -w 100 seq -t -c
STAT totalTime 497.40000000000043
```
```
$ ./raid.py -L 0 -n 100 -D 8 -w 100 seq -t -c
STAT totalTime 156.49999999999994

$ ./raid.py -L 1 -n 100 -D 8 -w 100 seq -t -c
STAT totalTime 275.69999999999993

$ ./raid.py -L 4 -n 100 -D 8 -w 100 seq -t -c
STAT totalTime 937.8571428571443

$ ./raid.py -L 5 -n 100 -D 8 -w 100 seq -t -c
STAT totalTime 290.9
```

8. Run the timing mode one last time, but this time with a sequential workload (-W sequential). How does the performance vary with RAID level, and when doing reads versus writes? How about when varying the size of each request? What size should you write to a RAID when using RAID-4 or RAID-5?

```
$ ./raid.py -L 0 -n 100 -D 4 -w 100 -W seq -t -c
STAT totalTime 12.499999999999991
275.69999999999993 / 12.499999999999991 = 22.056

$ ./raid.py -L 1 -n 100 -D 4 -w 100 -W seq -t -c
STAT totalTime 14.999999999999982
473.6000000000004 / 14.999999999999982 = 31.5733333333

$ ./raid.py -L 4 -n 100 -D 4 -w 100 -W seq -t -c
STAT totalTime 13.399999999999988
386.1000000000002 / 13.399999999999988 = 28.8134328358

$ ./raid.py -L 5 -n 100 -D 4 -w 100 -W seq -t -c
STAT totalTime 13.399999999999988
276.7 / 13.399999999999988 = 20.6492537313
```
```
$ ./raid.py -L 0 -n 100 -D 8 -w 100 -W seq -t -c
STAT totalTime 11.299999999999995
156.49999999999994 / 11.299999999999995 = 13.8495575221

$ ./raid.py -L 1 -n 100 -D 8 -w 100 -W seq -t -c
STAT totalTime 12.499999999999991
258.8999999999999 / 12.499999999999991 = 20.712

$ ./raid.py -L 4 -n 100 -D 8 -w 100 -W seq -t -c
STAT totalTime 11.514285714285709
164.99999999999994 / 11.514285714285709 = 14.3300248139

$ ./raid.py -L 5 -n 100 -D 8 -w 100 -W seq -t -c
STAT totalTime 11.499999999999995
158.59999999999997 / 11.499999999999995 = 13.7913043478
```
