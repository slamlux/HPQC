# HPQC Week2

Hello world:
Running the hello world program in python and c both took relatively short times but the C code ran the code 9 times faster than the python code shown in the real line with both programs taking a similar amount of time sys line, the main difference being in the user line where the python code took 0.022 seconds and the C code took 0 seconds.

Repeat adder:
  Python code:
Input 1: 2, 3 	  Input 2: 123456789, 4	  Input3: 4, 123456789
real    0m0.018s	real    0m10.230s	      real    0m0.020s
user    0m0.005s	user    0m6.841s	      user    0m0.013s
sys     0m0.013s	sys     0m3.383s	      sys     0m0.005s

  C code:
Input 1: 2, 3 	  Input 2: 123456789, 4	  Input3: 4, 123456789
real    0m0.020s	real    0m0.323s	      real    0m0.003s
user    0m0.000s	user    0m0.320s	      user    0m0.000s
sys     0m0.002s	sys     0m0.001s	      sys     0m0.002s


For the repeat adder program when the first input is very large the run time of the code increases significantly in the case shown above for python program by 3 orders of magnitude. The C programs run much faster than the python code as demonstrated by input 2 and 5 in the above table for a large first input which the code runs slower for both C and Python.  

Time print:
For the C code printing to the command line, there is no significant differences in the loop run time inside the code and for the “time” command in the command line, for inputs of 10 and 200. With similar times also for outputting to a file for the inputs of 10 and 200 but having a longer run time than printing to the command line but not by much
