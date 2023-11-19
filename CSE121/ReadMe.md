# Knight's Tour Solver and Tester
Course Project for [CSE121 - *Discrete Mathematics*](http://techtree.iiitd.edu.in/viewDescription/filename?=CSE121) (Monsoon 2023), IIIT Delhi.  
Here's a Knight's Tour Solver based on Warnsdorff’s algorithm implemented in C, along with automated testing routines.

## Instructions
The solver code is in `KnightsTour.c`.  
The tester code is in `KnightsTourTester.c`.

0) To evaluate, ensure that you have UNIX terminal with `make` and `gcc` installed.
1) Execute `make` in the directory where files are unzipped.
2) Run knight's tour solver with `./tour <rows> <cols> <initial_x> <initial_y>` .  
Example: `./tour 8 8 0 0` to check knight's tour on 8x8 board with (0,0) as initial point.
3) Run automated tests with `./test <rows> <cols>` .  
Example: `./test 8 8` to check knight's tour on 8x8 with every square as initial point.

## Miscellaneous
1) We have introduced a soft cap of 25 over values of rows and columns, as after this it takes a lot more time.
2) Sample cases to avoid worst case time complexity:  
mxn, where m<=6 or n<=6  
mxn, where either m or n (or both) is(are) even.