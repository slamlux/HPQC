# HPQC week2

# How to run the code

**time_print_in.py**:

Python code to read from a file and print to the command line
from the home dirctory input:  

    time python3 HPQC/week2_tasks/time_print_in.py input_txt_file (path to the txt file to use as input e.g. data#/number.txt)

the input file should have integer to input on the first line of the file 

**time_print_out.py:**

python code to print the outpit of time_print to a file

from the home dirctory input:  

    time python3 HPQC/week2_tasks/time_print_out.py input (e.g. 10)

inputs should be integers, the program will output the file to data#/test.txt so have a directory in the home directory called data# for the program to work

**time_print_in.c:**

C code to read from a file and print to the command line

from the home dirctory input:

    gcc HPQC/week2_tasks/time_print_in.c -o bin/time_print_in

then to run the code

    time bin/time_print_in input_txt_file (path to the txt file to use as input)

the input file should have integer to input on the first line of the file 

**time_print_out.c:**

C code to print the outpit of time_print to a file

from the home dirctory input:

    gcc HPQC/week2_tasks/time_print_out.c -o bin/time_print_out

then to run the code

    time bin/time_print_out input (e.g. 10) > output file (path to the txt file to use as input e.g. data#/number.txt)

this command will create the text file if it doesn't exist already 

# conclusions

Hello world: Running the hello world program in python and c both took relatively short times but the C code ran the code 9 times faster than the python code shown in the real line with both programs taking a similar amount of time sys line, the main difference being in the user line where the python code took 0.022 seconds and the C code took 0 seconds.

## Repeat adder: 
Python code: 

![](https://github.com/slamlux/HPQC/blob/main/week2_tasks/repeat_adder_python.png)

C code: 

![](https://github.com/slamlux/HPQC/blob/main/week2_tasks/repeat_adder_C.png)

For the repeat adder program when the first input is very large the run time of the code increases significantly in the case shown above for python program by 3 orders of magnitude. The C programs run much faster than the python code as demonstrated by input 2 and 5 in the above table for a large first input which the code runs slower for both C and Python.

## Time print:
For the C code printing to the command line, there is no significant differences in the loop run time inside the code and for the “time” command in the command line, for inputs of 10 and 200. With similar times also for outputting to a file for the inputs of 10 and 200 but having a longer run time than printing to the command line but not by much
If you input from a file the time to run the loop from the time function in the code is much shorter than for outputting to a file or to the command line: the former being 0.00001s and the latter being 0.000135

For the python taking the input from a file takes longer than outputting to the command line or to a file. Using a file as input and outputting to a file have similar user line of the time command while it being the lowest when printing to the command line. 
