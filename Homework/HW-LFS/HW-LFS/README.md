# Homework (Simulation)
This section introduces lfs.py, a simple LFS simulator you can use
to understand better how an LFS-based file system works. Read the
README for details on how to run the simulator.

```
prompt> ./lfs.py -h
Usage: lfs.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -N, --no_force        Do not force checkpoint writes after updates
  -D, --use_disk_cr     use disk (maybe old) version of checkpoint region
  -c, --compute         compute answers for me
  -o, --show_operations
                        print out operations as they occur
  -i, --show_intermediate
                        print out state changes as they occur
  -e, --show_return_codes
                        show error/return codes
  -n NUM_COMMANDS, --num_commands=NUM_COMMANDS
                        generate N random commands
  -p PERCENTAGES, --percentages=PERCENTAGES
                        percent chance of:
                        createfile,writefile,createdir,rmfile,linkfile,sync
                        (example is c30,w30,d10,r20,l10,s0)
  -a INODE_POLICY, --allocation_policy=INODE_POLICY
                        inode allocation policy: "r" for "random" or "s" for
                        "sequential"
  -L COMMAND_LIST, --command_list=COMMAND_LIST
                        command list in format:
                        "cmd1,arg1,...,argN:cmd2,arg1,...,argN:... where cmds
                        are:c:createfile, d:createdir, r:delete, w:write,
                        l:link, s:syncformat: c,filepath d,dirpath r,filepath
                        w,filepath,offset,numblks l,srcpath,dstpath s

```
## Questions
1. Run ./lfs.py -n 3, perhaps varying the seed (-s). Can you figure out which commands were run to generate the final file system contents? Can you tell which order those commands were issued? Finally, can you determine the liveness of each block in the final file system state? Use -o to show which commands were run, and -c to show the liveness of the final file system state. How much harder does the task become for you as you increase the number of commands issued (i.e., change -n 3 to -n 5)?

```
$ ./lfs.py -n 3 -o -c -s 0
```
```
$ ./lfs.py -n 3 -o -c -s 1
```
```
$ ./lfs.py -n 4 -o -c -s 0
```
```
$ ./lfs.py -n 5 -o -c -s 0
```
2. If you find the above painful, you can help yourself a little bit by showing the set of updates caused by each specific command. To do so, run ./lfs.py -n 3 -i. Now see if it is easier to understand what each command must have been. Change the random seed to get different commands to interpret (e.g., -s 1, -s 2, -s 3, etc.).
```
$ ./lfs.py -n 3 -o -c -i -s 0
```
create file ( /ku3 ) -> lseek ( /ku3, 7*block_size ) -> write file( /ku3 offset=7 size=4) ....

```
$ ./lfs.py -n 3 -o -c -i -s 1
```
```
$ ./lfs.py -n 4 -o -c -i -s 0
```
```
$ ./lfs.py -n 5 -o -c -i -s 0
```

3. To further test your ability to figure out what updates are made to disk by each command, run the following: ./lfs.py -o -F -s 100 (and perhaps a few other random seeds). This just shows a set of commands and does NOT show you the final state of the file system. Can you reason about what the final state of the file system must be?
```
$ ./lfs.py -o -F -s 100
```
create file /us7
write file  /us7 offset=4 size=0
write file  /us7 offset=7 size=7

It create a file called us7 and write to this file twice 

```
$ ./lfs.py -o -F -s 1
```
```
$ ./lfs.py -o -F -s 0
```

4. Now see if you can determine which files and directories are live after a number of file and directory operations. Run tt ./lfs.py -n 20 -s 1 and then examine the final file system state. Can you figure out which pathnames are valid? Run tt ./lfs.py -n 20 -s 1 -c -v to see the results. Run with -o to see if your answers match up given the series of random commands. Use different random seeds to get more problems.
```
$ ./lfs.py -n 20 -s 1
```
```
$ ./lfs.py -n 20 -s 1 -c -v -o
```
Live directories:  ['/ln7']
Live files:  ['/lt0', '/af4', '/ln7/zp3', '/ln7/zu5']
```
$ ./lfs.py -n 20 -s 0 -c -v -o
```
Live directories:  ['/cl6']
Live files:  ['/qg9', '/wg3', '/cl6/so8', '/ep6']

5. Now let’s issue some specific commands. First, let’s create a file and write to it repeatedly. To do so, use the -L flag, which lets you specify specific commands to execute. Let’s create the file ”/foo” and write to it four times: -L c,/foo:w,/foo,0,1:w,/foo,1,1:w,/foo,2,1:w,/foo,3,1 -o. See if you can determine the liveness of the final file system state; use -c to check your answers.
```
$ ./lfs.py -L c,/foo:w,/foo,0,1:w,/foo,1,1:w,/foo,2,1:w,/foo,3,1 -o -c
```
6. Now, let’s do the same thing, but with a single write operation instead of four. Run ./lfs.py -o -L c,/foo:w,/foo,0,4 to create file ”/foo” and write 4 blocks with a single write operation. Compute the liveness again, and check if you are right with -c. What is the main difference between writing a file all at once (as we do here) versus doing it one block at a time (as above)? What does this tell you about the importance of buffering updates in main memory as the real LFS does?
```
$ ./lfs.py -o -L c,/foo:w,/foo,0,4 -c 
```
Buffering make the read and write faster.
Creating one block at a time create more garbage and separate the data blocks

7. Let’s do another specific example. First, run the following: ./lfs.py -L c,/foo:w,/foo,0,1. What does this set of commands do? Now, run ./lfs.py -L c,/foo:w,/foo,7,1. What does this set of commands do? How are the two different? What can you tell about the size field in the inode from these two sets of commands?
```
$ ./lfs.py -L c,/foo:w,/foo,0,1 -o -v -c
```
create file /foo
write file  /foo offset=0 size=1
Live directories:  []
Live files:  ['/foo']

Write one block at the beginning.
size is 1.
```
$ ./lfs.py -L c,/foo:w,/foo,7,1 -o -v -c
```
create file /foo
write file  /foo offset=7 size=1
size inode: 10
Live directories:  []
Live files:  ['/foo']

Write one block at the end.
size is 8.

8. Now let’s look explicitly at file creation versus directory creation. Run simulations ./lfs.py -L c,/foo and ./lfs.py -L d,/foo to create a file and then a directory. What is similar about these runs, and what is different?
```
$ ./lfs.py -L c,/foo
```
```
$ ./lfs.py -L d,/foo
```
Create a file does not have a data block at first, but directory has one inmediately and also increase one reference to the root directory.

9. The LFS simulator supports hard links as well. Run the following to study how they work: ./lfs.py -L c,/foo:l,/foo,/bar:l,/foo,/goo -o -i. What blocks are written out when a hard link is created? How is this similar to just creating a new file, and how is it different? How does the reference count field change as links are created?
```
$ ./lfs.py -L c,/foo:l,/foo,/bar:l,/foo,/goo -o -i
```
create file /foo
link file   /foo /bar
link file   /foo /goo

Blocks written out when a hard link is created:
Checkpoint region, parent directory data block and inode, imap.
Same: both update checkpoint region, parent directory data block and inode, imap.
Different: create new file will create a new inode for it, create hard link doesn't create new inode.
the reference count field change as links are created adding by one

10. LFS makes many different policy decisions. We do not explore many of them here – perhaps something left for the future – but here is a simple one we do explore: the choice of inode number. First, run ./lfs.py -p c100 -n 10 -o -a s to show the usual behavior with the ”sequential” allocation policy, which tries to use free inode numbers nearest to zero. Then, change to a ”random” policy by running ./lfs.py -p c100 -n 10 -o -a r (the -p c100 flag ensures 100 percent of the random operations are file creations). What on-disk differences does a random policy versus a sequential policy result in? What does this say about the importance of choosing inode numbers in a real LFS?
```
$ ./lfs.py -p c100 -n 10 -o -a s
```
```
$ ./lfs.py -p c100 -n 10 -o -a r
```
Each imap has 16 entries.
When assigning sequentially, all 10 files can be taken care of by the imap 0. So only one imap will be written for each update write.
If it is out of order and the inode number of the new file exceeds 15, the new imap will be enabled, and the root directory will be updated, So there is a high probability that creating a file will write two imaps.
Therefore is better if the files and directories are in the same imap. 

11. One last thing we’ve been assuming is that the LFS simulator always updates the checkpoint region after each update. In the real LFS, that isn’t the case: it is updated periodically to avoid long seeks. Run ./lfs.py -N -i -o -s 1000 to see some operations and the intermediate and final states of the file system when the checkpoint region isn’t forced to disk. What would happen if the checkpoint region is never updated? What if it is updated periodically? Could you figure out how to recover the file system to the latest state by rolling forward in the log?
```
$ ./lfs.py -N -i -o -s 1000
```
All operations will have no meaning if the checkpoint region is never updated. the consequences will be data lost.
It should be update periodically.
The latest log has the latest imap, use it to update the checkpoint region.
The roll-forward strategy is to read the latest CR, find a new one, and then continue to see if the subsequent update is legal, and update it if it is legal. 
