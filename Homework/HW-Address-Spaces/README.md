# Homework (Code)
In this homework, we’ll just learn about a few useful tools to examine virtual memory usage on Linux-based systems. This will only be a brief hint at what is possible; you’ll have to dive deeper on your own to truly become an expert (as always!).
 
 ## Questions:
 
1. The first Linux tool you should check out is the very simple tool free. First, type man free and read its entire manual page; it’s short, don’t worry!
```
$ man free

MALLOC(3)                BSD Library Functions Manual                MALLOC(3)

NAME
     calloc, free, malloc, realloc, reallocf, valloc, aligned_alloc -- memory allocation
```

2. Now, run free, perhaps using some of the arguments that might be useful (e.g., -m, to display memory totals in megabytes). How much memory is in your system? How much is free? Do these numbers match your intuition?

```
$ vm_stat
```
```
Mach Virtual Memory Statistics: (page size of 4096 bytes)
Pages free:                                5093.
Pages active:                            444277.
Pages inactive:                          440620.
Pages speculative:                         3498.
Pages throttled:                              0.
Pages wired down:                        528329.
Pages purgeable:                           6625.
"Translation faults":                 510496275.
Pages copy-on-write:                   20624767.
Pages zero filled:                    251389791.
Pages reactivated:                     34002739.
Pages purged:                          11836362.
File-backed pages:                       288589.
Anonymous pages:                         599806.
Pages stored in compressor:             2645555.
Pages occupied by compressor:            675038.
Decompressions:                        48482378.
Compressions:                          63903512.
Pageins:                               39333690.
Pageouts:                                264615.
Swapins:                                8847092.
Swapouts:                               9440713.
```

```
 vm_stat | perl -ne '/page size of (\d+)/ and $size=$1; /Pages\s+([^:]+)[^\d]+(\d+)/ and printf("%-16s % 16.2f Mi\n", "$1:", $2 * $size / 1048576);'
```
```
free:                       29.13 Mi
active:                   1587.07 Mi
inactive:                 1513.05 Mi
speculative:                66.62 Mi
throttled:                   0.00 Mi
wired down:               2192.08 Mi
purgeable:                   0.27 Mi
copy-on-write:           80615.70 Mi
zero filled:            982862.75 Mi
reactivated:            133051.93 Mi
purged:                  46302.73 Mi
stored in compressor:         10549.46 Mi
occupied by compressor:          2803.04 Mi
```
```
pip install psutil
curl -O https://raw.githubusercontent.com/giampaolo/psutil/master/scripts/meminfo.py
python ./meminfo.py
```
```
MEMORY
------
Total      :    8.0G
Available  :    1.6G
Percent    :    80.0
Used       :    3.7G
Free       :   18.7M
Active     :    1.6G
Inactive   :    1.6G
Wired      :    2.1G

SWAP
----
Total      :    3.0G
Used       :    1.5G
Free       :    1.5G
Percent    :    48.4
Sin        :  150.4G
Sout       :    1.0G
```

3. Next, create a little program that uses a certain amount of memory, called memory-user.c. This program should take one commandline argument: the number of megabytes of memory it will use. When run, it should allocate an array, and constantly stream through the array, touching each entry. The program should do this indefinitely, or, perhaps, for a certain amount of time also specified at the command line.


4. Now, while running your memory-user program, also (in a different terminal window, but on the same machine) run the free tool. How do the memory usage totals change when your program is running? How about when you kill the memory-user program? Do the numbers match your expectations? Try this for different amounts of memory usage. What happens when you use really large amounts of memory?

before:
```
vm_stat
```
```
Mach Virtual Memory Statistics: (page size of 4096 bytes)
Pages free:                               49828.
Pages active:                            536037.
Pages inactive:                          515002.
Pages speculative:                        16037.
Pages throttled:                              0.
Pages wired down:                        537114.
```
```
$ ./memory-user 1000000 10
```
```
vm_stat

```
after:
```
Mach Virtual Memory Statistics: (page size of 4096 bytes)
Pages free:                               10555.
Pages active:                            589773.
Pages inactive:                          578654.
Pages speculative:                        15973.
Pages throttled:                              0.
Pages wired down:                        482196.
```

5. Let’s try one more tool, known as pmap. Spend some time, and read the pmap manual page in detail.


6. To use pmap, you have to know the process ID of the process you’re interested in. Thus, first run ps auxw to see a list of all processes; then, pick an interesting one, such as a browser. You can also use your memory-user program in this case (indeed, you can even have that program call getpid() and print out its PID for your convenience).

[link memeory map in macOS](https://jvns.ca/blog/2018/01/26/mac-memory-maps/)
the vmmap with a PID as an argument will give you a detailed report on all regions mapped by the process,

```
man vmmap
 vmmap -- Display the virtual memory regions allocated in a process
```

7. Now run pmap on some of these processes, using various flags (like-X) to reveal many details about the process. What do you see? How many different entities make up a modern address space, as opposed to our simple conception of code/stack/heap?
```
vmmap --wide 42929
```
```
 ==== Summary for process 42929
 ReadOnly portion of Libraries: Total=637.8M resident=108.3M(17%) swapped_out_or_unallocated=529.5M(83%)
 Writable regions: Total=64.3M written=5716K(9%) resident=1892K(3%) swapped_out=4456K(7%) unallocated=58.1M(90%)

                                 VIRTUAL RESIDENT    DIRTY  SWAPPED VOLATILE   NONVOL    EMPTY   REGION 
 REGION TYPE                        SIZE     SIZE     SIZE     SIZE     SIZE     SIZE     SIZE    COUNT (non-coalesced) 
 ===========                     ======= ========    =====  ======= ========   ======    =====  =======  
Kernel Alloc Once                    8K       4K       4K       4K       0K       0K       0K        1 
....
__DATA                            40.1M    30.6M     274K    1199K       0K       0K       0K      388 
__DATA_CONST                       459K     243K      16K     152K       0K       0K       0K       30 
__FONT_DATA                          4K       0K       0K       0K       0K       0K       0K        1 
__LINKEDIT                       388.8M    15.0M       0K       0K       0K     
....
===========                     ======= ========    =====  ======= ========   ======    =====  ======= 
TOTAL                            861.3M   165.5M    2396K    5959K       0K       8K    4352K      972 

                                 VIRTUAL   RESIDENT      DIRTY    SWAPPED ALLOCATION      BYTES DIRTY+SWAP          REGION
MALLOC ZONE                         SIZE       SIZE       SIZE       SIZE      COUNT  ALLOCATED  FRAG SIZE  % FRAG   COUNT
===========                      =======  =========  =========  =========  =========  =========  =========  ======  ======
DefaultMallocZone_0x10d48f000      37.1M      1756K      1756K      3764K      33880      4606K       914K     17%      18
NWMallocZone_0x10d5fb000           1024K         8K         8K         4K         11       1712        10K     87%       1
===========                      =======  =========  =========  =========  =========  =========  =========  ======  ======
TOTAL                              38.1M      1764K      1764K      3768K      33891      4607K       925K     17%      19
```
8. Finally, let’s run pmap on your memory-user program, with different amounts of used memory. What do you see here? Does the output from pmap match your expectations?
```
vmmap --wide 44254
```
```
==== Summary for process 44254
ReadOnly portion of Libraries: Total=393.0M resident=18.5M(5%) swapped_out_or_unallocated=374.5M(95%)
Writable regions: Total=1.1G written=37.9M(3%) resident=37.9M(3%) swapped_out=0K(0%) unallocated=1.1G(97%)

                                VIRTUAL RESIDENT    DIRTY  SWAPPED VOLATILE   NONVOL    EMPTY   REGION
REGION TYPE                        SIZE     SIZE     SIZE     SIZE     SIZE     SIZE     SIZE    COUNT (non-coalesced)
===========                     ======= ========    =====  ======= ========   ======    =====  =======
Kernel Alloc Once                    8K       4K       4K       0K       0K       0K       0K        1
MALLOC guard page                   16K       0K       0K       0K       0K       0K       0K        4
MALLOC metadata                     44K      36K      36K       0K       0K       0K       0K        5
MALLOC_LARGE                     128.0M    37.8M    37.8M       0K       0K       0K       0K        1         see MALLOC ZONE table below
MALLOC_LARGE (reserved)            1.0G       0K       0K       0K       0K       0K       0K        1         see MALLOC ZONE table below
MALLOC_LARGE metadata                4K       4K       4K       0K       0K       0K       0K        1         see MALLOC ZONE table below
MALLOC_SMALL                      8192K       8K       8K       0K       0K       0K       0K        1         see MALLOC ZONE table below
MALLOC_TINY                       1024K      16K      16K       0K       0K       0K       0K        1         see MALLOC ZONE table below
STACK GUARD                       56.0M       0K       0K       0K       0K       0K       0K        1
Stack                             8192K      20K      20K       0K       0K       0K       0K        2
__DATA                             600K     356K     164K       0K       0K       0K       0K       40
__DATA_CONST                        24K      24K      24K       0K       0K       0K       0K        2
__LINKEDIT                       388.3M    15.0M       0K       0K       0K       0K       0K        3
__OBJC_RO                         32.2M    19.6M       0K       0K       0K       0K       0K        1
__OBJC_RW                         1892K    1144K       4K       0K       0K       0K       0K        2
__TEXT                            4752K    3552K       0K       0K       0K       0K       0K       40
shared memory                        8K       8K       8K       0K       0K       0K       0K        2
unused but dirty shlib __DATA        4K       4K       4K       0K       0K       0K       0K       12
===========                     ======= ========    =====  ======= ========   ======    =====  =======
TOTAL                              1.6G    77.5M    38.1M       0K       0K       0K       0K      120

                                 VIRTUAL   RESIDENT      DIRTY    SWAPPED ALLOCATION      BYTES DIRTY+SWAP          REGION
MALLOC ZONE                         SIZE       SIZE       SIZE       SIZE      COUNT  ALLOCATED  FRAG SIZE  % FRAG   COUNT
===========                      =======  =========  =========  =========  =========  =========  =========  ======  ======
DefaultMallocZone_0x102a0d000     137.0M      37.9M      37.9M         0K        156       1.1G         0K      0%       4
```
