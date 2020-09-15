# Homework (Simulation)
In this homework, you will use a simple program, which is known as paging-linear-translate.py, to see if you understand how simple virtual-to-physical address translation works with linear page tables. See the README for details.

```
$ ./paging-linear-translate.py -h
```
```
Usage: paging-linear-translate.py [options]

Options:
  -h, --help            show this help message and exit
  -A ADDRESSES, --addresses=ADDRESSES
                        a set of comma-separated pages to access; -1 means
                        randomly generate
  -s SEED, --seed=SEED  the random seed
  -a ASIZE, --asize=ASIZE
                        address space size (e.g., 16, 64k, 32m, 1g)
  -p PSIZE, --physmem=PSIZE
                        physical memory size (e.g., 16, 64k, 32m, 1g)
  -P PAGESIZE, --pagesize=PAGESIZE
                        page size (e.g., 4k, 8k, whatever)
  -n NUM, --numaddrs=NUM
                        number of virtual addresses to generate
  -u USED, --used=USED  percent of virtual address space that is used
  -v                    verbose mode
  -c                    compute answers for me
```
- The -s flag changes the random seed and thus generates different
  page table values as well as different virtual addresses to translate.
- The -a flag changes the size of the address space.
- The -p flag changes the size of physical memory.
- The -P flag changes the size of a page.
- The -n flag can be used to generate more addresses to translate
  (instead of the default 5).
- The -u flag changes the fraction of mappings that are valid, from
  0% (-u 0) up to 100% (-u 100). The default is 50, which means
  that roughly 1/2 of the pages in the virtual address space will be valid.
- The -v flag prints out the VPN numbers to make your life easier.

## Questions:

1. Before doing any translations, let’s use the simulator to study how linear page tables change size given different parameters. Compute the size of linear page tables as different parameters change. Some suggested inputs are below; by using the -v flag, you can see how many page-table entries are filled. First, to understand how linear page table size changes as the address space grows, run with these flags:

```
 $ ./paging-linear-translate.py -P 1k -a 1m -p 512m -v -n 0
 $ ./paging-linear-translate.py -P 1k -a 2m -p 512m -v -n 0
 $ ./paging-linear-translate.py -P 1k -a 4m -p 512m -v -n 0
```
Then, to understand how linear page table size changes as page size grows:

```
 $ ./paging-linear-translate.py -P 1k -a 1m -p 512m -v -n 0
 $ ./paging-linear-translate.py -P 2k -a 1m -p 512m -v -n 0
 $ ./paging-linear-translate.py -P 4k -a 1m -p 512m -v -n 0
```
Before running any of these, try to think about the expected trends. How should page-table size change as the address space grows? As the page size grows? Why not use big pages in general?

page-table size = address space / page size

Page table size increases as the address space grows because we need more pages to cover the whole address space. 
When Page sizes increases the page table size decreases because we the page have more space in size to cover the whole address space.

Big pages => waste of memory and slower machine.

2. Now let’s do some translations. Start with some small examples, and change the number of pages that are allocated to the address space with the -u flag. For example:

```
 $ ./paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 0
 $ ./paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 25
 $ ./paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 50
 $ ./paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 75
 $ ./paging-linear-translate.py -P 1k -a 16k -p 32k -v -u 100
```
What happens as you increase the percentage of pages that are allocated in each address space?

(The -u flag changes the fraction of mappings that are valid, from 0% (-u 0) up to 100% (-u 100). The default is 50, which means that roughly 1/2 of the pages in the virtual address space will be valid.)

As the percentage of pages increase more memory access operations become valid however free space decreases.


3. Now let’s try some different random seeds, and some different (and sometimes quite crazy) address-space parameters, for variety:
```
 $ ./paging-linear-translate.py -P 8 -a 32 -p 1024 -v -s 1
 $ ./paging-linear-translate.py -P 8k -a 32k -p 1m -v -s 2
 $ ./paging-linear-translate.py -P 1m -a 256m -p 512m -v -s 3
```
Which of these parameter combinations are unrealistic? Why?
The first is too small and the third is too big.

4. Use the program to try out some other problems. Can you find the limits of where the program doesn’t work anymore? For example, what happens if the address-space size is bigger than physical memory?

```
$ ./paging-linear-translate.py -a 65k -v -c
ARG address space size 65k
ARG phys mem size 64k
Error: physical memory size must be GREATER than address space size (for this simulation)

$ ./paging-linear-translate.py -a 0 -v -c
ARG address space size 0
Error: must specify a non-zero address-space size.

$  ./paging-linear-translate.py -p 0 -v -c
ARG phys mem size 0
Error: must specify a non-zero physical memory size.

$ ./paging-linear-translate.py -P 0 -v -c
Traceback (most recent call last):
File "./paging-linear-translate.py", line 85, in <module>
    mustbemultipleof(asize, pagesize, 'address space must be a multiple of the pagesize')
File "./paging-linear-translate.py", line 14, in mustbemultipleof
    if (int(float(bignum)/float(num)) != (int(bignum) / int(num))):
ZeroDivisionError: float division by zero

$ ./paging-linear-translate.py -P 32k -v -c
Traceback (most recent call last):
File "./paging-linear-translate.py", line 174, in <module>
    if pt[vpn] < 0:
IndexError: array index out of range
```
Some limits to look up:

- page size is greater than address-space.
- address space size is greater than the physical memory.
- physical memory size is not multiple of page size.
- address space is not multiple of page size.
- page size is negative.
- physical memory is negative.
- address space is negative.
