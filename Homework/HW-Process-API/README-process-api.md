#  Homework (Code)

In this homework, you are to gain some familiarity with the process management APIs about which you just read. Don’t worry – it’s even more fun than it sounds! You’ll in general be much better off if you find as much time as you can to write some code, so why not start now?

## Questions

1. Write a program that calls fork(). Before calling fork(), have the main process access a variable (e.g., x) and set its value to something (e.g., 100).

What value is the variable in the child process?

The value in the child process is:

`x in child process: 101`

What happens to the variable when both the child and parent change the value of x?

`x in parent process: 102
 x in child process: 101`

the value in the variable x is different for child and parent

Each mantain their own copy of the variable.

fork(  ) creates copy of parent process. However, child and parent process have their own private address space exclusive of each other. Both process can't interfare in each other's address space (memory).

2. Write a program that opens a file (with the open() system call) and then calls fork() to create a new process.

Can both the child and parent access the file descriptor returned by open()? 

Yes, both can access the file descriptor opened using open( ).

What happens when they are writing to the file concurrently, i.e., at the same time?

One overwrites the other one.
Both are able  to write to the file but the order in wich they do is un-deterministic (if we dont use wait).

3. Write another program using fork(). The child process should print “hello”; the parent process should print “goodbye”. You should try to ensure that the child process always prints first; can you do this without calling wait() in the parent?

Can we use the command sleep -> I use in the parent process and it also work.

Can we the command kill( ) to send SIGCONT signal to parent process.

4. Write a program that calls fork() and then calls some form of exec() to run the program /bin/ls. See if you can try all of the variants of exec(), including (on Linux) execl(), execle(), execlp(), execv(), execvp(), and execvpe(). Why do you think there are so many variants of the same basic call?

The exec family of functions are ultimately a system call. System calls go straight into the kernel and typically perform a very specific service that only the kernel can do.

- execl and execv are nearly identical
- execlp and execvp are similar
- execle and execvpe are similar 

Rules
--------
-> Functions having lin their name need argument separated by commas
-> Functions having v in their name need arguments as (v-> vector (array))
-> Functions having e need environment argument

[some exec information](https://www.mkssoftware.com/docs/man3/execl.3.asp).

[An important video](https://www.youtube.com/watch?v=OVFEWSP7n8c)

The execlp(), execvp(), and execvpe() functions duplicate the actions of the shell in searching for an executable file if the specified file name does not contain a slash (/) character.
The file is sought in the colon-separated list of directory pathnames specified in the PATH environment variable.

If/when execvp finds a match, that program will be loaded into memory and replace your current running program.

The arguments the new program will see are the argv array specified in execvp. You are expected to have a null pointer as the last element, or your program may crash looking for a null.

5. Now write a program that uses wait() to wait for the child process to finish in the parent.

[Important link](https://stackoverflow.com/questions/49459392/developing-a-correct-understanding-of-waitpid-and-getpid).

What does wait() return? What happens if you use wait() in the child?

`hello, I am child (rc_wait: -1), (pid:36528)
hello, I am parent of 36528, (rc_wait: 36528), (pid:36527)`

If any process has no child process then wait() returns immediately “-1”.

If only one child process is terminated, then return a wait() returns process ID of the terminated child process.

If we use wait( ) in child process then wait( ) return -1. Because, there is no child of child. So , there is no wait for any process to exit.

[some informative link](https://www.geeksforgeeks.org/wait-system-call-c/)

6. Write a slight modification of the previous program, this time using waitpid() instead of wait(). When would waitpid() be useful?

Return value of waitpid()

- pid of child, if child has exited
- 0, if using WNOHANG and child hasn’t exited

The wait function can block the caller until a child process terminates, whereas waitpid has an option that prevents it from blocking.

waitpid is used to block the execution of a process (unless WNOHANG is passed) and resume execution when a (or more) child of the process ends. waitpid returns the pid of the child whose state has changed, -1 on failure. It also can return 0 if WNOHANG has specified but the child has not changed the state.

waitpid=> is used when we want to wait for a specific child process rather than aiting for all children process to exit and it also allow us to specify more behaviours.

7. Write a program that creates a child process, and then in the child closes standard output (STDOUT FILENO). What happens if the child calls printf() to print some output after closing the descriptor?

The call of printf in the child process wont show in the terminal because we close the standard output before that call.

8. Write a program that creates two children, and connects the standard output of one to the standard input of the other, using the pipe() system call.

[some informative link](https://www.geeksforgeeks.org/c-program-demonstrate-fork-and-pipe/)

[An important video](https://www.youtube.com/watch?v=Mqb2dVRe0uo)
