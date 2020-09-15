# Homework (Simulation)
This section introduces fsck.py, a simple simulator you can use to
better understand how file system corruptions can be detected (and potentially repaired). Please see the associated README for details on how to run the simulator.

```
$ ./fsck.py -h
 Options:
   -h, --help            show this help message and exit
   -s SEED, --seed=SEED  first random seed (for a filesystem)
   -S SEEDCORRUPT, --seedCorrupt=SEEDCORRUPT
                         second random seed (for corruptions)
   -i NUMINODES, --numInodes=NUMINODES
                         number of inodes in file system
   -d NUMDATA, --numData=NUMDATA
                         number of data blocks in file system
   -n NUMREQUESTS, --numRequests=NUMREQUESTS
                         number of requests to simulate
   -p, --printFinal      print the final set of files/dirs
   -w WHICHCORRUPT, --whichCorrupt=WHICHCORRUPT
                         do a specific corruption
   -c, --compute         compute answers for me
   -D, --dontCorrupt     don't actually corrupt file system
```
## Questions
1. First, run fsck.py -D; this flag turns off any corruption, and thus you can use it to generate a random file system, and see if you can determine which files and directories are in there. So, go ahead and do that! Use the -p flag to see if you were right. Try this for a few different randomly-generated file systems by setting the seed (-s) to different values, like 1, 2, and 3.

```
$ ./fsck.py -D -s 1 -p
```
```
$ ./fsck.py -D -s 2 -p
```
```
$ ./fsck.py -D -s 3 -p
```
2. Now, let’s introduce a corruption. Run fsck.py -S 1 to start. Can you see what inconsistency is introduced? How would you fix it in a real file system repair tool? Use -c to check if you were right.
```
$ ./fsck.py -S 1 -c
```
This exercise is explain in the README of this problem.
CORRUPTION::INODE BITMAP corrupt bit 13.
the inode bitmap was changed in bit 13, marking that bit free when actually the inode is allocated.

3. Change the seed to -S 3 or -S 19; which inconsistency do you see? Use -c to check your answer. What is different in these two cases?
```
$ ./fsck.py -S 3 -c
```
CORRUPTION::INODE 11 orphan
```
$ ./fsck.py -S 19 -c
```
CORRUPTION::INODE 0 points to dead block 15

4. Change the seed to -S 5; which inconsistency do you see? How hard would it be to fix this problem in an automatic way? Use -c to check your answer. Then, introduce a similar inconsistency with -S 38; is this harder/possible to detect? Finally, use -S 642; is this inconsistency detectable? If so, how would you fix the file system?
```
$ ./fsck.py -S 5 -c
```
CORRUPTION::INODE 4 orphan
The inode [d a:-1 r:1] directory data address is set to -1.
fix this giving address of the data to this directory.

```
$ ./fsck.py -S 38 -c
```
CORRUPTION::INODE 10 points to dead block 11
 The data bitmap  1100100110001010 in inode 10 the bit is set to 0 this is the way to detect this inconsistencies.
I dont know how to fix it.

```
$ ./fsck.py -S 642 -c
```
CORRUPTION::INODE 8 points to dead block 11

The data bitmap  1100100110001010 in inode 8  the bit is set to 0 this is the way to detect this inconsistencies.
I dont know how to fix it.

5. Change the seed to -S 6 or -S 13; which inconsistency do you see? Use -c to check your answer. What is the difference across these two cases? What should the repair tool do when encountering such a situation?
```
$ ./fsck.py -S 6 -c
```
CORRUPTION::INODE 14 orphan
```
$ ./fsck.py -S 13 -c
```
CORRUPTION::INODE 9 points to dead block 5

6. Change the seed to -S 9; which inconsistency do you see? Use -c to check your answer. Which piece of information should a checkand-repair tool trust in this case?
```
$ ./fsck.py -S 9 -c
```
CORRUPTION::INODE 15 with directory [('.', 15), ('..', 0), ('l', 6), ('q', 8), ('j', 3), ('c', 9)]:entry ('l', 6) altered to refer to different name (i)

7. Change the seed to -S 15; which inconsistency do you see? Use -c to check your answer. What can a repair tool do in this case? If no repair is possible, how much data is lost?
```
$ ./fsck.py -S 15 -c
```
CORRUPTION::INODE 1 orphan
All data will be lost.

8. Change the seed to -S 10; which inconsistency do you see? Use -c to check your answer. Is there redundancy in the file system structure here that can help a repair?
```
$ ./fsck.py -S 10 -c
```
CORRUPTION::INODE 1 orphan

9. Change the seed to -S 16 and -S 20; which inconsistency do you see? Use -c to check your answer. How should the repair tool fix the problem?
```
$ ./fsck.py -S 16 -c
```
CORRUPTION::INODE 12 was type file, now dir
All data will be lost.
We need to change the type to file
```
$ ./fsck.py -S 20 -c
```
CORRUPTION::INODE 6 was type file, now dir
