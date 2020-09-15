# Homework (Code)
In this homework, you will gain some familiarity with memory allocation. First, you’ll write some buggy programs (fun!). Then, you’ll use some tools to help you find the bugs you inserted. Then, you will realize
how awesome these tools are and use them in the future, thus making
yourself more happy and productive. The tools are the debugger (e.g.,
gdb) and a memory-bug detector called valgrind [SN05].


## Questions:

1. First, write a simple program called null.c that creates a pointer to an integer, sets it to NULL, and then tries to dereference it. Compile this into an executable called null. What happens when you run this program?

    Segmentation fault: 11

2. Next, compile this program with symbol information included (with the -g flag). Doing so let’s put more information into the executable, enabling the debugger to access more useful information about variable names and the like. Run the program under the debugger by typing gdb null and then, once gdb is running, typing run. What does gdb show you?
```
Program received signal SIGSEGV, Segmentation fault.
0x0000000100000f59 in main () at null.c:9
9           printf("%d\n", *x);
```
| Signal | Value | Action | Comment|
| :---: | :---: | :---: | :---: |
|SIGSEGV |    11    |  Core | Invalid memory reference|

3. Finally, use the valgrind tool on this program. We’ll use the memcheck tool that is a part of valgrind to analyze what happens. Run this by typing in the following: valgrind --leak-check=yes null. What happens when you run this? Can you interpret the output from the tool?
```
valgrind --leak-check=yes ./null

Invalid read of size 4 ==27805== at 0x4004E6: main (null.c:9) ==27805== Address 0x0 is not stack'd, malloc'd or (recently) free'd
```

4. Write a simple program that allocates memory using malloc() but forgets to free it before exiting. What happens when this program runs? Can you use gdb to find any problems with it? How about valgrind (again with the --leak-check=yes flag)?

When we run this program nothing bad happen.
valgrind pointed out that we allocates 4 bytes but we don't freed them on exit. So, we lost 4 bytes.

5. Write a program that creates an array of integers called data of size 100 using malloc; then, set data[100] to zero. What happens when you run this program? What happens when you run this program using valgrind? Is the program correct?

Valgrind says Invalid write of size 4.(Mean we write something on wrong place)

and 400 bytes are lost (because we don't freed the memory we assigned)

I think we are overwriting someone else' memory.

6. Create a program that allocates an array of integers (as above), frees them, and then tries to print the value of one of the elements of the array. Does the program run? What happens when you use valgrind on it?
```
==27852== Invalid read of size 4
==27852==    at 0x104D0: main (free_then_print.c:8)
==27852==  Address 0x49c6028 is 0 bytes inside a block of size 100 free'd
==27852==    at 0x4848B8C: free (vg_replace_malloc.c:530)
==27852==    by 0x104CB: main (free_then_print.c:6)
==27852==  Block was alloc'd at
==27852==    at 0x4847568: malloc (vg_replace_malloc.c:299)
==27852==    by 0x104BB: main (free_then_print.c:5)
```


7. Now pass a funny value to free (e.g., a pointer in the middle of the array you allocated above). What happens? Do you need tools to find this type of problem?

free(): invalid pointer
[1]    29093 abort (core dumped)  ./free_then_print.out

it says "free() invalid pointer" and crashed. We don't need any tool to find out the problem.

8. Try out some of the other interfaces to memory allocation. For example, create a simple vector-like data structure and related routines that use realloc() to manage the vector. Use an array to store the vectors elements; when a user adds an entry to the vector, use realloc() to allocate more space for it. How well does such a vector perform? How does it compare to a linked list? Use valgrind to help you find bugs.

[Implementing a Dynamic Vector (Array) in C](https://eddmann.com/posts/implementing-a-dynamic-vector-array-in-c/)

9. Spend more time and read about using gdb and valgrind. Knowing your tools is critical; spend the time and learn how to become an expert debugger in the UNIX and C environment.
