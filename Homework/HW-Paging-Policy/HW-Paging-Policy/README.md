# Homework (Simulation)
This simulator, paging-policy.py, allows you to play around with
different page-replacement policies. See the README for details.

```
$ ./paging-policy.py -h
```
```
Usage: paging-policy.py [options]

Options:
  -h, --help            show this help message and exit
  -a ADDRESSES, --addresses=ADDRESSES
                        a set of comma-separated pages to access; -1 means
                        randomly generate
  -f ADDRESSFILE, --addressfile=ADDRESSFILE
                        a file with a bunch of addresses in it
  -n NUMADDRS, --numaddrs=NUMADDRS
                        if -a (--addresses) is -1, this is the number of addrs
                        to generate
  -p POLICY, --policy=POLICY
                        replacement policy: FIFO, LRU, OPT, UNOPT, RAND, CLOCK
  -b CLOCKBITS, --clockbits=CLOCKBITS
                        for CLOCK policy, how many clock bits to use
  -C CACHESIZE, --cachesize=CACHESIZE
                        size of the page cache, in pages
  -m MAXPAGE, --maxpage=MAXPAGE
                        if randomly generating page accesses, this is the max
                        page number
  -s SEED, --seed=SEED  random number seed
  -N, --notrace         do not print out a detailed trace
  -c, --compute         compute answers for me
```

## Questions
1. Generate random addresses with the following arguments: -s 0 -n 10, -s 1 -n 10, and -s 2 -n 10. Change the policy from FIFO, to LRU, to OPT. Compute whether each access in said address traces are hits or misses.
```
$ ./paging-policy.py -s 0 -n 10 -c
```
```
$ ./paging-policy.py -s 0 -n 10 --policy LRU -c
```
```
$ ./paging-policy.py -s 0 -n 10 --policy OPT -c
```
```
$ ./paging-policy.py -s 1 -n 10 -C
```
```
$ ./paging-policy.py -s 1 -n 10 --policy LRU -c
```
```
$ ./paging-policy.py -s 1 -n 10 --policy OPT -c
```
```
$ ./paging-policy.py -s 2 -n 10 -c
```
```
$ ./paging-policy.py -s 2 -n 10 --policy LRU -c
```
```
$ ./paging-policy.py -s 2 -n 10 --policy OPT -c
```
2. For a cache of size 5, generate worst-case address reference streams for each of the following policies: FIFO, LRU, and MRU (worst-case reference streams cause the most misses possible. For the worst case reference streams, how much bigger of a cache is needed to improve performance dramatically and approach OPT?
```
$ ./paging-policy.py --addresses=0,1,2,3,4,5,0,1,2,3,4,5 --cachesize=5 -c
```
```
./paging-policy.py --addresses=0,1,2,3,4,5,0,1,2,3,4,5 --policy=LRU --cachesize=5 -c
```
```
./paging-policy.py --addresses=0,1,2,3,4,5,0,1,2,3,4,5 --policy=MRU --cachesize=5 -c
```
The smaller the cache in a worst-case reference streams the bigger improve the performance approachin OPT.

3. Generate a random trace (use python or perl). How would you expect the different policies to perform on such a trace?

In the case of RANDOM policy there is no hystorical feature, and in there is no difference in the case of FIFO, LRU and RAN

```
$ ./paging-policy.py -s 1 -n 10 -c

FINALSTATS hits 2   misses 8   hitrate 20.00
```
```
$ ./paging-policy.py -s 1 -n 10 -c --policy=LRU
FINALSTATS hits 2   misses 8   hitrate 20.00
```
```
$ ./paging-policy.py -s 1 -n 10 -c --policy=OPT
FINALSTATS hits 3   misses 7   hitrate 30.00
```
```
$ ./paging-policy.py -s 1 -n 10 -c --policy=UNOPT
FINALSTATS hits 2   misses 8   hitrate 20.00
```
```
$ ./paging-policy.py -s 1 -n 10 -c --policy=RAND
FINALSTATS hits 2   misses 8   hitrate 20.00
```
```
$ ./paging-policy.py -s 1 -n 10 -c --policy=CLOCK
FINALSTATS hits 2   misses 8   hitrate 20.00
```
```
$ ./paging-policy.py -s 1 -n 10 -c --policy=MRU
FINALSTATS hits 2   misses 8   hitrate 20.00
```

4. Now generate a trace with some locality. How can you generate such a trace? How does LRU perform on it? How much better than RAND is LRU? How does CLOCK do? How about CLOCK with different numbers of clock bits?

```
$ ./generatetrace_locality.py
[3, 0, 6, 6, 6, 6, 7, 0, 6, 6]
```

```
$ ./paging-policy.py --addresses=3,0,6,6,6,6,7,0,6,6 --policy=LRU -c
```
```
Access: 3  MISS LRU ->          [3] <- MRU Replaced:- [Hits:0 Misses:1]
Access: 0  MISS LRU ->       [3, 0] <- MRU Replaced:- [Hits:0 Misses:2]
Access: 6  MISS LRU ->    [3, 0, 6] <- MRU Replaced:- [Hits:0 Misses:3]
Access: 6  HIT  LRU ->    [3, 0, 6] <- MRU Replaced:- [Hits:1 Misses:3]
Access: 6  HIT  LRU ->    [3, 0, 6] <- MRU Replaced:- [Hits:2 Misses:3]
Access: 6  HIT  LRU ->    [3, 0, 6] <- MRU Replaced:- [Hits:3 Misses:3]
Access: 7  MISS LRU ->    [0, 6, 7] <- MRU Replaced:3 [Hits:3 Misses:4]
Access: 0  HIT  LRU ->    [6, 7, 0] <- MRU Replaced:- [Hits:4 Misses:4]
Access: 6  HIT  LRU ->    [7, 0, 6] <- MRU Replaced:- [Hits:5 Misses:4]
Access: 6  HIT  LRU ->    [7, 0, 6] <- MRU Replaced:- [Hits:6 Misses:4]

FINALSTATS hits 6   misses 4   hitrate 60.00
```

```
$ ./paging-policy.py --addresses=3,0,6,6,6,6,7,0,6,6 --policy=RAND -c

```
```
Access: 3  MISS Left  ->          [3] <- Right Replaced:- [Hits:0 Misses:1]
Access: 0  MISS Left  ->       [3, 0] <- Right Replaced:- [Hits:0 Misses:2]
Access: 6  MISS Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:0 Misses:3]
Access: 6  HIT  Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:1 Misses:3]
Access: 6  HIT  Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:2 Misses:3]
Access: 6  HIT  Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:3 Misses:3]
Access: 7  MISS Left  ->    [3, 0, 7] <- Right Replaced:6 [Hits:3 Misses:4]
Access: 0  HIT  Left  ->    [3, 0, 7] <- Right Replaced:- [Hits:4 Misses:4]
Access: 6  MISS Left  ->    [3, 0, 6] <- Right Replaced:7 [Hits:4 Misses:5]
Access: 6  HIT  Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:5 Misses:5]

FINALSTATS hits 5   misses 5   hitrate 50.00

```
```
$ ./paging-policy.py --addresses=3,0,6,6,6,6,7,0,6,6 --policy=CLOCK --clockbits=0 -c 
```
```
Access: 3  MISS Left  ->          [3] <- Right Replaced:- [Hits:0 Misses:1]
Access: 0  MISS Left  ->       [3, 0] <- Right Replaced:- [Hits:0 Misses:2]
Access: 6  MISS Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:0 Misses:3]
Access: 6  HIT  Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:1 Misses:3]
Access: 6  HIT  Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:2 Misses:3]
Access: 6  HIT  Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:3 Misses:3]
Access: 7  MISS Left  ->    [3, 0, 7] <- Right Replaced:6 [Hits:3 Misses:4]
Access: 0  HIT  Left  ->    [3, 0, 7] <- Right Replaced:- [Hits:4 Misses:4]
Access: 6  MISS Left  ->    [3, 7, 6] <- Right Replaced:0 [Hits:4 Misses:5]
Access: 6  HIT  Left  ->    [3, 7, 6] <- Right Replaced:- [Hits:5 Misses:5]

FINALSTATS hits 5   misses 5   hitrate 50.00
```
```
$ ./paging-policy.py --addresses=3,0,6,6,6,6,7,0,6,6 --policy=CLOCK --clockbits=1 -c 
```
```
Access: 3  MISS Left  ->          [3] <- Right Replaced:- [Hits:0 Misses:1]
Access: 0  MISS Left  ->       [3, 0] <- Right Replaced:- [Hits:0 Misses:2]
Access: 6  MISS Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:0 Misses:3]
Access: 6  HIT  Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:1 Misses:3]
Access: 6  HIT  Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:2 Misses:3]
Access: 6  HIT  Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:3 Misses:3]
Access: 7  MISS Left  ->    [3, 0, 7] <- Right Replaced:6 [Hits:3 Misses:4]
Access: 0  HIT  Left  ->    [3, 0, 7] <- Right Replaced:- [Hits:4 Misses:4]
Access: 6  MISS Left  ->    [3, 7, 6] <- Right Replaced:0 [Hits:4 Misses:5]
Access: 6  HIT  Left  ->    [3, 7, 6] <- Right Replaced:- [Hits:5 Misses:5]

FINALSTATS hits 5   misses 5   hitrate 50.00
```
```
$ ./paging-policy.py --addresses=3,0,6,6,6,6,7,0,6,6 --policy=CLOCK --clockbits=2 -c 
```
```
Access: 3  MISS Left  ->          [3] <- Right Replaced:- [Hits:0 Misses:1]
Access: 0  MISS Left  ->       [3, 0] <- Right Replaced:- [Hits:0 Misses:2]
Access: 6  MISS Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:0 Misses:3]
Access: 6  HIT  Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:1 Misses:3]
Access: 6  HIT  Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:2 Misses:3]
Access: 6  HIT  Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:3 Misses:3]
Access: 7  MISS Left  ->    [3, 6, 7] <- Right Replaced:0 [Hits:3 Misses:4]
Access: 0  MISS Left  ->    [3, 7, 0] <- Right Replaced:6 [Hits:3 Misses:5]
Access: 6  MISS Left  ->    [7, 0, 6] <- Right Replaced:3 [Hits:3 Misses:6]
Access: 6  HIT  Left  ->    [7, 0, 6] <- Right Replaced:- [Hits:4 Misses:6]

FINALSTATS hits 4   misses 6   hitrate 40.00
```
```
$ ./paging-policy.py --addresses=3,0,6,6,6,6,7,0,6,6 --policy=CLOCK --clockbits=3 -c 
```
```
Access: 3  MISS Left  ->          [3] <- Right Replaced:- [Hits:0 Misses:1]
Access: 0  MISS Left  ->       [3, 0] <- Right Replaced:- [Hits:0 Misses:2]
Access: 6  MISS Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:0 Misses:3]
Access: 6  HIT  Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:1 Misses:3]
Access: 6  HIT  Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:2 Misses:3]
Access: 6  HIT  Left  ->    [3, 0, 6] <- Right Replaced:- [Hits:3 Misses:3]
Access: 7  MISS Left  ->    [3, 6, 7] <- Right Replaced:0 [Hits:3 Misses:4]
Access: 0  MISS Left  ->    [6, 7, 0] <- Right Replaced:3 [Hits:3 Misses:5]
Access: 6  HIT  Left  ->    [6, 7, 0] <- Right Replaced:- [Hits:4 Misses:5]
Access: 6  HIT  Left  ->    [6, 7, 0] <- Right Replaced:- [Hits:5 Misses:5]

FINALSTATS hits 5   misses 5   hitrate 50.00
```
5. Use a program like valgrind to instrument a real application and generate a virtual page reference stream. For example, running valgrind --tool=lackey --trace-mem=yes ls will output a nearly-complete reference trace of every instruction and data reference made by the program ls. To make this useful for the simulator above, you’ll have to first transform each virtual memory reference into a virtual page-number reference (done by masking off the offset and shifting the resulting bits downward). How big of a cache is needed for your application trace in order to satisfy a large fraction of requests? Plot a graph of its working set as the size of the cache increases.
