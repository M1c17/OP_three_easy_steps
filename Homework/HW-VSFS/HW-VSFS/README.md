# Homework (Simulation)
Use this tool, vsfs.py, to study how file system state changes as various operations take place. The file system begins in an empty state, with
just a root directory. As the simulation takes place, various operations are
performed, thus slowly changing the on-disk state of the file system. See
the README for details.

The possible operations are:

- mkdir() - creates a new directory
- creat() - creates a new (empty) file
- open(), write(), close() - appends a block to a file
- link()   - creates a hard link to a file
- unlink() - unlinks a file (removing it if linkcnt==0)

The state of the file
system is shown by printing the contents of four different data structures:

inode bitmap - indicates which inodes are allocated
inodes       - table of inodes and their contents
data bitmap  - indicates which data blocks are allocated
data         - indicates contents of data blocks

```
$ ./vsfs.py -h
Usage: vsfs.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -i NUMINODES, --numInodes=NUMINODES
                        number of inodes in file system
  -d NUMDATA, --numData=NUMDATA
                        number of data blocks in file system
  -n NUMREQUESTS, --numRequests=NUMREQUESTS
                        number of requests to simulate
  -r, --reverse         instead of printing state, print ops
  -p, --printFinal      print the final set of files/dirs
  -c, --compute         compute answers for me
```
A few other flags control various aspects of the simulation, including the
number of inodes ("-i"), the number of data blocks ("-d"), and whether to
print the final list of all directories and files in the file system ("-p").

## Questions

1. Run the simulator with some different random seeds (say 17, 18, 19, 20), and see if you can figure out which operations must have taken place between each state change.
```
$ ./vsfs.py -n 3 -s 17
```
```
$ ./vsfs.py -n 3 -s 18
```
```
$ ./vsfs.py -n 3 -s 19
```
```
$ ./vsfs.py -n 3 -s 20
```
2. Now do the same, using different random seeds (say 21, 22, 23, 24), except run with the -r flag, thus making you guess the state change while being shown the operation. What can you conclude about the inode and data-block allocation algorithms, in terms of which blocks they prefer to allocate?
```
$ ./vsfs.py -n 3 -s 21 -r
```
```
$ ./vsfs.py -n 3 -s 22 -r
```
```
$ ./vsfs.py -n 3 -s 23 -r
```
```
$ ./vsfs.py -n 3 -s 24 -r
```
inode and data-block allocation algorithm prefer allocate first available block.
Allocate inode and data space in order

3. Now reduce the number of data blocks in the file system, to very low numbers (say two), and run the simulator for a hundred or so requests. What types of files end up in the file system in this highlyconstrained layout? What types of operations would fail?
```
$ ./vsfs.py -n 100 -s 24 -d 2 -c
```
Inititial state with a directory with some empty files and 2 links allocated
failed error: 
mkdir("/z");
File system out of data blocks; rerun with more via command-line flag?

Only creat\unlink can succed.
write and mkdir must fail 

4. Now do the same, but with inodes. With very few inodes, what types of operations can succeed? Which will usually fail? What is the final state of the file system likely to be?
```
$ ./vsfs.py -n 100 -s 24 -i 2 -c
```
the final state:
directory with some empty files and links allocated

failed error:
All operations except unlink( ) will fail. 
Only the first inode is available
