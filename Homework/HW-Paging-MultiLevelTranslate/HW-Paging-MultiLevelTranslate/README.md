# Homework (Simulation)
This fun little homework tests if you understand how a multi-level
page table works. And yes, there is some debate over the use of the term
“fun” in the previous sentence. The program is called, perhaps unsurprisingly: paging-multilevel-translate.py; see the README for
details.

```
$ ./paging-multilevel-translate.py -h
```
```
Usage: paging-multilevel-translate.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -a ALLOCATED, --allocated=ALLOCATED
                        number of virtual pages allocated
  -n NUM, --addresses=NUM
                        number of virtual addresses to generate
  -c, --solve           compute answers for me
```
## Questions

1. With a linear page table, you need a single register to locate the page table, assuming that hardware does the lookup upon a TLB miss. How many registers do you need to locate a two-level page table? A three-level table?

Assuming that there are thus three base/bounds pairs, one each for code, heap, and stack. When a process is running, the base register for each of these segments contains the physical address of a linear page table for that segment; thus, each process in the system now has
three page tables associated with it.
For a two-level page table we need 2 base/bounds pairs.

2. Use the simulator to perform translations given random seeds 0, 1, and 2, and check your answers using the -c flag. How many memory references are needed to perform each lookup?

```
$ ./paging-multilevel-translate.py -s 0 -c
```
4
```
$ ./paging-multilevel-translate.py -s 1 -c
```
5
```
$ ./paging-multilevel-translate.py -s 2 -c
```
5

3. Given your understanding of how cache memory works, how do you think memory references to the page table will behave in the cache? Will they lead to lots of cache hits (and thus fast accesses?) Or lots of misses (and thus slow accesses)?

It doesn't fit temporal locality or spatial locality, so will cause slow accesses.
