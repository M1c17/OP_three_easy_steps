# Homework (Simulation)
This section introduces ffs.py, a simple FFS simulator you can use to understand better how FFS-based file and directory allocation work. See the README for details on how to run the simulator. 
```
./ffs.py -h

Usage: ffs.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -n NUM_GROUPS, --num_groups=NUM_GROUPS
                        number of block groups
  -d BLOCKS_PER_GROUP, --datablocks_per_groups=BLOCKS_PER_GROUP
                        data blocks per group
  -i INODES_PER_GROUP, --inodes_per_group=INODES_PER_GROUP
                        inodes per group
  -L LARGE_FILE_EXCEPTION, --large_file_exception=LARGE_FILE_EXCEPTION
                        0:off, N>0:blocks in group before spreading file to
                        next group
  -f INPUT_FILE, --input_file=INPUT_FILE
                        command file
  -I, --spread_inodes   Instead of putting file inodes in parent dir group,
                        spread them evenly around all groups
  -D, --spread_data     Instead of putting data near inode,
                        spread them evenly around all groups
  -A ALLOCATE_FARAWAY, --allocate_faraway=ALLOCATE_FARAWAY
                        When picking a group, examine this many groups at a
                        time
  -C CONTIG_ALLOCATION_POLICY, --contig_allocation_policy=CONTIG_ALLOCATION_POLICY
                        number of contig free blocks needed to alloc
  -T, --show_spans      show file and directory spans
  -M, --show_symbol_map
                        show symbol map
  -B, --show_block_addresses
                        show block addresses alongside groups
  -S, --do_per_file_stats
                        print out detailed inode stats
  -v, --show_file_ops   print out detailed per-op success/failure
  -c, --compute         compute answers for me
  ```
  ## Questions
1. Examine the file in.largefile, and then run the simulator with flag -f in.largefile and -L 4. The latter sets the large-file exception to 4 blocks. What will the resulting allocation look like? Run with -c to check.
```
$ ./ffs.py -f in.largefile -L 4 -c
group inodes
data
0 /a-------- /aaaa----- ---------- ----------

1 ---------- aaaa------ ---------- ----------

2 ---------- aaaa------ ---------- ----------

3 ---------- aaaa------ ---------- ----------

4 ---------- aaaa------ ---------- ----------

5 ---------- aaaa------ ---------- ----------

6 ---------- aaaa------ ---------- ----------

7 ---------- aaaa------ ---------- ----------

8 ---------- aaaa------ ---------- ----------

9 ---------- aaaa------ ---------- ----------
```
Each group has 4 a data blocks.

2. Now run with -L 30. What do you expect to see? Once again, turn on -c to see if you were right. You can also use -S to see exactly which blocks were allocated to the file /a.
```
$ ./ffs.py -f in.largefile -L 30 -S -c
group inodes
data
0 /a-------- /aaaaaaaaa aaaaaaaaaa aaaaaaaaaa

1 ---------- aaaaaaaaaa a--------- ----------

2 ---------- ---------- ---------- ----------

3 ---------- ---------- ---------- ----------

4 ---------- ---------- ---------- ----------

5 ---------- ---------- ---------- ----------

6 ---------- ---------- ---------- ----------

7 ---------- ---------- ---------- ----------

8 ---------- ---------- ---------- ----------

9 ---------- ---------- ---------- ----------
```
In this case 30 data blocks in each group.
This is the same as not adding the large file exception option.

3. Now we will compute some statistics about the file. The first is something we call filespan, which is the max distance between any two data blocks of the file or between the inode and any data block. Calculate the filespan of /a. Run ffs.py -f in.largefile -L 4 -T -c to see what it is. Do the same with -L 100. What difference do you expect in filespan as the large-file exception parameter changes from low values to high values?
```
$ ./ffs.py -f in.largefile -L 4 -T -c
span: files
  file:         /a  filespan: 372
               avg  filespan: 372.00

span: directories
  dir:           /  dirspan: 373
               avg  dirspan: 373.00
```
```
$ ./ffs.py -f in.largefile -L 100 -T -c
span: files
  file:         /a  filespan:  59
               avg  filespan:  59.00

span: directories
  dir:           /  dirspan:  60
               avg  dirspan:  60.00
```
The larger the L, the smaller the range of the data block occupied by the file and the tighter the distribution

4. Now let’s look at a new input file, in.manyfiles. How do you think the FFS policy will lay these files out across groups? (you can run with -v to see what files and directories are created, or just cat in.manyfiles). Run the simulator with -c to see if you were right.
```
$ ./ffs.py -f in.manyfiles -v -c
```
inodes and data blocks of the files in the same directory should be in the same group.
Convention principle, separate directories, the same directory file in one group

5. A metric to evaluate FFS is called dirspan. This metric calculates the spread of files within a particular directory, specifically the max distance between the inodes and data blocks of all files in the directory and the inode and data block of the directory itself. Run with in.manyfiles and the -T flag, and calculate the dirspan of the three directories. Run with -c to check. How good of a job does FFS do in minimizing dirspan?
```
$ ./ffs.py -f in.manyfiles -T -c
span: files
avg  filespan:  14.76
span: directories
avg  dirspan:  27.33
```
The maximum distance between the directory inode and the inodes and blocks of all files in the directory. Because they are all in the same group, it is the length of the group.

6. Now change the size of the inode table per group to 5 (-I 5). How do you think this will change the layout of the files? Run with -c to see if you were right. How does it affect the dirspan?
```
$ ./ffs.py -f in.manyfiles -I 5 -T -c
span: files
avg  filespan:  11.92
span: directories
avg  dirspan: 358.00
```
The files in the same directory are spread to differents groups.
the dirspan will increase.

7. Which group should FFS place inode of a new directory in? The default (simulator) policy looks for the group with the most free inodes. A different policy looks for a set of groups with the most free inodes. For example, if you run with -A 2, when allocating a new directory, the simulator will look at groups in pairs and pick the best pair for the allocation. Run ./ffs.py -f in.manyfiles -I 5 -A 2 -c to see how allocation changes with this strategy. How does it affect dirspan? Why might this policy be good?
```
$ ./ffs.py -f in.manyfiles -I 5 -A 2 -T -c
span: directories
dir:           /  dirspan: 333
dir:          /j  dirspan: 335
dir:          /t  dirspan: 336
             avg  dirspan: 334.67
```
It keeps different directories away. It will decrease dirspan. 
It make files in the same directory nearer when the inodes per group is small.

8. One last policy change we will explore relates to file fragmentation. Run ./ffs.py -f in.fragmented -v and see if you can predict how the files that remain are allocated. Run with -c to confirm your answer. What is interesting about the data layout of file /i? Why is it problematic?
```
$ ./ffs.py -f in.fragmented -v -c
```
It gets fragmented instead of continuos blocks.
read and write will be slow

9. A new policy, which we call contiguous allocation (-C), tries to ensure that each file is allocated contiguously. Specifically, with -C n, the file system tries to ensure that n contiguous blocks are free within a group before allocating a block. Run ./ffs.py -f in.fragmented -v -C 2 -c to see the difference. How does layout change as the parameter passed to -C increases? Finally, how does -C affect filespan and dirspan?
```
$ ./ffs.py -f in.fragmented -v -C 2 -T -c
span: files
  file:         /b  filespan:  10
  file:         /d  filespan:  10
  file:         /f  filespan:  10
  file:         /h  filespan:  10
  file:         /i  filespan:  25
               avg  filespan:  13.00

span: directories
  dir:           /  dirspan:  26
               avg  dirspan:  26.00
```
Now file /i is located contiguously
dirspan and filespan will decrease.
