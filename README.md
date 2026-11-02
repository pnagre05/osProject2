Project 2
Group Number: 5
Group Members: Pranav Nagre, Huzaifa Nizami Ahmad

To compile the program, a makefile is provided, which provides the basis of the make commands. There are two commands within the makefile:
- make clean
  -  a command used to delete temporary build files created during a software compilation, restoring the directory to a clean state for a fresh start
- make
  - this command compiles the code, displaying errors if present, or getting it ready for execution if otherwise

The steps to run the program are as follows
- Run "make clean"
- Run "make"
- enter "./my-dining" followed by a number

Deadlock Prevention
For the coarse-grained solution, we use a single Tournament-Tree lock to control access to the eating section, allowing only one philosopher to eat at a time.
For the fine-grained solution, we use one Peterson lock per chopstick. Each philosopher must acquire both adjacent chopsticks before eating.
To prevent deadlock, we use a resource hierarchy strategy. Every philosopher acquires the chopstick with the smaller ID first, followed by the chopstick with the larger ID. The locks are released in reverse order.
This consistent ordering prevents circular waiting, eliminating the possibility of deadlock caused by philosophers waiting on one another's chopsticks.

Assumptions
-in the event there is not a number specified when executing the code, then the code is not supposed to run
-The program needs at least 2 philosophers
Limitations
- Since the tournament tree is represented with a binary tree, then the implementation calculates the next power of two that is greater than or equal to n.
  If n is not a power of two, the tree allocates memory for unused dummy leaf nodes and locks to maintain the tree's structure. THis does not effect the functionality of the code.
