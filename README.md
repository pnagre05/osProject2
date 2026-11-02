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

In order to prevent deadlocks, we chose to implement a strategy that involves a resource hierarchy. Instead of using a global lock, we chose to lock each mutex/semaphore individually.
We also wanted to make sure that we do not run into any circular waits, which in the context of the philosopher problem is when all the philosophers pick their left fork
simultaneously. To do this, we enforced a rule that the first n-1 philosophers pick up there left fork first, while the last one chooses his right fork first, then the left.
This guarantees that at least one philosopher will always be able to acquire both forks, complete their critical section (eating), and release the resources for others, fully preventing system deadlock.

Assumptions
-in the event there is not number specified when executing the code, the default is that there are 5 philosophers
