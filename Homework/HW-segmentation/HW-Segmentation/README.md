# Homework (Simulation)
This program allows you to see how address translations are performed
in a system with segmentation. See the README for details.

```
./segmentation.py -h
```
```
Usage: segmentation.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -A ADDRESSES, --addresses=ADDRESSES
                        a set of comma-separated pages to access; -1 means
                        randomly generate
  -a ASIZE, --asize=ASIZE
                        address space size (e.g., 16, 64k, 32m, 1g)
  -p PSIZE, --physmem=PSIZE
                        physical memory size (e.g., 16, 64k, 32m, 1g)
  -n NUM, --numaddrs=NUM
                        number of virtual addresses to generate
  -b BASE0, --b0=BASE0  value of segment 0 base register
  -l LEN0, --l0=LEN0    value of segment 0 limit register
  -B BASE1, --b1=BASE1  value of segment 1 base register
  -L LEN1, --l1=LEN1    value of segment 1 limit register
  -c                    compute answers for me
```

## Questions:

1. First let’s use a tiny address space to translate some addresses. Here’s a simple set of parameters with a few different random seeds; can you translate the addresses? segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 0 segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 1 segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 2

```
$./segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 0 -c
```
```
ARG seed 0
ARG address space size 128
ARG phys mem size 512

Segment register information:

  Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
  Segment 0 limit                  : 20

  Segment 1 base  (grows negative) : 0x00000200 (decimal 512)
  Segment 1 limit                  : 20

Virtual Address Trace
  VA  0: 0x0000006c (decimal:  108) --> VALID in SEG1: 0x000001ec (decimal:  492)
  VA  1: 0x00000061 (decimal:   97) --> SEGMENTATION VIOLATION (SEG1)
  VA  2: 0x00000035 (decimal:   53) --> SEGMENTATION VIOLATION (SEG0)
  VA  3: 0x00000021 (decimal:   33) --> SEGMENTATION VIOLATION (SEG0)
  VA  4: 0x00000041 (decimal:   65) --> SEGMENTATION VIOLATION (SEG1)
```
-> VA  0: 0x0000006c (decimal:  108) --> VALID in SEG1: 0x000001ec (decimal:  492)

1) 108 = 1101100
2) top bit => 1 = segment 1 
3) offset => 101100 = 44
4) PA = BASE + Offset - MaxSegment
    PA = 512    +    44    -    ? 
5) MaxSegment = 2^[log2(address size) - number of segment bits]
    Max Segment = 2^(log2(128) - 1) = 2^(7 - 1) = 2^(6) = 64

4) But first
PA = BASE + Offset - MaxSegment
PA =   512    +    44    -    ? 
_____________________________________
So, our Offset calculatio:

Offset = Decimal Representation of 0b101100 - 64

Offset = 44 - 64 = -20
Absolute of Offset should be less or equal than Bounds(Limit)

|-20| <= 20 True (Valid)
______________________________________

PA =   512    +    44    -    ? 
PA =   512    +    44    -   64  = 492 
_____________________________________

```
$./segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 1 -c 
```
```
ARG seed 1
ARG address space size 128
ARG phys mem size 512

Segment register information:

  Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
  Segment 0 limit                  : 20

  Segment 1 base  (grows negative) : 0x00000200 (decimal 512)
  Segment 1 limit                  : 20

Virtual Address Trace
  VA  0: 0x00000011 (decimal:   17) --> VALID in SEG0: 0x00000011 (decimal:   17)
  VA  1: 0x0000006c (decimal:  108) --> VALID in SEG1: 0x000001ec (decimal:  492)
  VA  2: 0x00000061 (decimal:   97) --> SEGMENTATION VIOLATION (SEG1)
  VA  3: 0x00000020 (decimal:   32) --> SEGMENTATION VIOLATION (SEG0)
  VA  4: 0x0000003f (decimal:   63) --> SEGMENTATION VIOLATION (SEG0)
```
->VA  0: 0x00000011 (decimal:   17) --> VALID in SEG0: 0x00000011 (decimal:   17)

1) 17 = 0010001
2) top bit => 0 = segment 0
3) offset => 010001 = 17
4) PA = BASE + Offset 
    PA =     0    +   17   = 17 
    
```
$./segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 2 -c
```
```
ARG seed 2
ARG address space size 128
ARG phys mem size 512

Segment register information:

  Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
  Segment 0 limit                  : 20

  Segment 1 base  (grows negative) : 0x00000200 (decimal 512)
  Segment 1 limit                  : 20

Virtual Address Trace
  VA  0: 0x0000007a (decimal:  122) --> VALID in SEG1: 0x000001fa (decimal:  506)
  VA  1: 0x00000079 (decimal:  121) --> VALID in SEG1: 0x000001f9 (decimal:  505)
  VA  2: 0x00000007 (decimal:    7) --> VALID in SEG0: 0x00000007 (decimal:    7)
  VA  3: 0x0000000a (decimal:   10) --> VALID in SEG0: 0x0000000a (decimal:   10)
  VA  4: 0x0000006a (decimal:  106) --> SEGMENTATION VIOLATION (SEG1)
```


2. Now, let’s see if we understand this tiny address space we’ve constructed (using the parameters from the question above). 
What is the highest legal virtual address in segment 0?

VA: 19

What about the lowest legal virtual address in segment 1?
VA: 108

What are the lowest and highest illegal addresses in this entire address space?

ARG address space size 128
lowest illegal = 20
```
$./segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 0 -c -A 20
```
```
ARG seed 0
ARG address space size 128
ARG phys mem size 512

Segment register information:

  Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
  Segment 0 limit                  : 20

  Segment 1 base  (grows negative) : 0x00000200 (decimal 512)
  Segment 1 limit                  : 20

Virtual Address Trace
  VA  0: 0x00000014 (decimal:   20) --> SEGMENTATION VIOLATION (SEG0)
```
highest illegal = 107
```
$ ./segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 0 -c -A 107
```
```
ARG seed 0
ARG address space size 128
ARG phys mem size 512

Segment register information:

  Segment 0 base  (grows positive) : 0x00000000 (decimal 0)
  Segment 0 limit                  : 20

  Segment 1 base  (grows negative) : 0x00000200 (decimal 512)
  Segment 1 limit                  : 20

Virtual Address Trace
  VA  0: 0x0000006b (decimal:  107) --> SEGMENTATION VIOLATION (SEG1)

Finally, how would you run segmentation.py with the -A flag to test if you are right?
```
3. Let’s say we have a tiny 16-byte address space in a 128-byte physical memory. What base and bounds would you set up so as to get the simulator to generate the following translation results for the specified address stream: valid, valid, violation, ..., violation, valid, valid? Assume the following parameters:
```
    segmentation.py -a 16 -p 128
                            -A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
                            --b0 ? --l0 ? --b1 ? --l1 ?
```
```
$ ./segmentation.py -a 16 -p 128 -A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 --b0 0 --l0 2 --b1 128 --l1 2 -c
```

4. Assume we want to generate a problem where roughly 90% of the randomly-generated virtual addresses are valid (not segmentation violations). How should you configure the simulator to do so? Which parameters are important to getting this outcome?
```
$ ./segmentation.py -a 128 -p 512 -b 0 -l 58 -B 512 -L 58 -s 0 -c
```
```
./segmentation.py -a 16 -p 128 -b0 0 --l0 7  -b1 128 --l1 7 -c
```
-l = (0.9 * -a)/2

5. Can you run the simulator such that no virtual addresses are valid? How?

```
$ ./segmentation.py -b 0 -l 0 -B 0 -L 0 -c
```
