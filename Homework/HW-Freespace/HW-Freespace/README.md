# Homework (Simulation)
The program, malloc.py, lets you explore the behavior of a simple free-space allocator as described in the chapter. See the README for details of its basic operation.

```
$ ./malloc.py -h
```
```
Usage: malloc.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -S HEAPSIZE, --size=HEAPSIZE
                        size of the heap
  -b BASEADDR, --baseAddr=BASEADDR
                        base address of heap
  -H HEADERSIZE, --headerSize=HEADERSIZE
                        size of the header
  -a ALIGNMENT, --alignment=ALIGNMENT
                        align allocated units to size; -1->no align
  -p POLICY, --policy=POLICY
                        list search (BEST, WORST, FIRST)
  -l ORDER, --listOrder=ORDER
                        list order (ADDRSORT, SIZESORT+, SIZESORT-, INSERT-
                        FRONT, INSERT-BACK)
  -C, --coalesce        coalesce the free list?
  -n OPSNUM, --numOps=OPSNUM
                        number of random ops to generate
  -r OPSRANGE, --range=OPSRANGE
                        max alloc size
  -P OPSPALLOC, --percentAlloc=OPSPALLOC
                        percent of ops that are allocs
  -A OPSLIST, --allocList=OPSLIST
                        instead of random, list of ops (+10,-0,etc)
  -c, --compute         compute answers for me
```
## Questions:
 
1. First run with the flags -n 10 -H 0 -p BEST -s 0 to generate a few random allocations and frees. 
Can you predict what alloc()/free() will return?

Yes, we can predict what alloc and free will return in the 10 random ops allocations since we have :
Base addrees -> baseAddr 1000
Header size -> headerSize 0

Can you guess the state of the free list after each request?
Yes, we can guess the state with all the given information it is possible.
What do you notice about the free list over time?

The memory is chop up into pieces because we are not coalescing the free list. We can turn on coalescing via the -C flag to avoid the fragmentation of the memory.
```
$ ./malloc.py -n 10 -H 0 -p BEST -s 0 -c
```

2. How are the results different when using a WORST fit policy to search the free list (-p WORST)? What changes?
```
$ ./malloc.py -n 10 -H 0 -p WORST -s 0 -c
```
(The worst fit approach is the opposite of best fit; find the largest chunk
and return the requested amount; keep the remaining (large) chunk on
the free list.)
The returned small block were not used until the larger free mem. blocks are not splitted and used.

3. What about when using FIRST fit (-p FIRST)? What speeds up when you use first fit?

```
$ ./malloc.py -n 10 -H 0 -p FIRST -s 0 -c
```
The first fit method simply finds the first block that is big enough and
returns the requested amount to the user.
No exhaustive search of all the free spaces are necessary. We just allocate the first fitting memory block to the job requestion memory in heap but sometimes pollutes the beginning of the free list with small objects

4. For the above questions, how the list is kept ordered can affect the time it takes to find a free location for some of the policies. Use the different free list orderings (-l ADDRSORT, -l SIZESORT+, -l SIZESORT-) to see how the policies and the list orderings interact.

```
$ ./malloc.py -n 10 -H 0 -p FIRST -s 0 -l ADDRSORT -c
``` 
```
$ ./malloc.py -n 10 -H 0 -p FIRST -s 0 -l SIZESORT+ -c
```
```
$ ./malloc.py -n 10 -H 0 -p FIRST -s 0 -l SIZESORT- -c
```
Here we can see kepping the list ordered decrease the time to find a free location because first fit chooses the appropriate size free location that come first.

5. Coalescing of a free list can be quite important. Increase the number of random allocations (say to -n 1000). What happens to larger allocation requests over time? Run with and without coalescing (i.e., without and with the -C flag). What differences in outcome do you see? How big is the free list over time in each case? Does the ordering of the list matter in this case?
```
$ ./malloc.py -n 1000 -r 20 -C -c
```
```
$ ./malloc.py -n 1000 -r 20 -c
```

Larger allocation requests in mechanism without coalescing fail because memory is chop up into small pieces. The free list became conjested with a lot of small free memory blocks. This conjestion also increase look up time in nearly all free mem because the free list is bigger and larger allocation requests will return NULL.  The ordering of the list matters only in first fit free mem.

6. What happens when you change the percent allocated fraction -P to higher than 50? What happens to allocations as it nears 100? What about as the percent nears 0?
```
$ ./malloc.py -c -n 1000 -P 100
$ ./malloc.py -c -n 1000 -P 1
```
-P OPSPALLOC, --percentAlloc=OPSPALLOC
percent of ops that are allocs

When we increase the percent allocated fraction to near 100 or 100 -> No more spaces to allocate; Then nearly all operations would be allocation operations. If we approach 0 -> the allocations and free operation are done in 50%. Because, we can't free memory that isn't allocated. So, in order to free mem we have to allocate some first.

7. What kind of specific requests can you make to generate a highlyfragmented free space? Use the -A flag to create fragmented free lists, and see how different policies and options change the organization of the free list.

```
$ ./malloc.py -c -A +20,+20,+20,+20,+20,-0,-1,-2,-3,-4
$ ./malloc.py -c -A +20,+20,+20,+20,+20,-0,-1,-2,-3,-4 -C
$ ./malloc.py -c -A +10,-0,+20,-1,+30,-2,+40,-3 -l SIZESORT-
$ ./malloc.py -c -A +10,-0,+20,-1,+30,-2,+40,-3 -l SIZESORT- -C
$ ./malloc.py -c -A +10,-0,+20,-1,+30,-2,+40,-3 -p FIRST -l SIZESORT+
$ ./malloc.py -c -A +10,-0,+20,-1,+30,-2,+40,-3 -p FIRST -l SIZESORT+ -C
$ ./malloc.py -c -A +10,-0,+20,-1,+30,-2,+40,-3 -p FIRST -l SIZESORT-
$ ./malloc.py -c -A +10,-0,+20,-1,+30,-2,+40,-3 -p FIRST -l SIZESORT- -C
$ ./malloc.py -c -A +10,-0,+20,-1,+30,-2,+40,-3 -p WORST -l SIZESORT+
$ ./malloc.py -c -A +10,-0,+20,-1,+30,-2,+40,-3 -p WORST -l SIZESORT+ -C
$ ./malloc.py -c -A +10,-0,+20,-1,+30,-2,+40,-3 -p WORST -l SIZESORT-
$ ./malloc.py -c -A +10,-0,+20,-1,+30,-2,+40,-3 -p WORST -l SIZESORT- -C

```
