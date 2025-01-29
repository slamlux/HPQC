

import sys
import time

def main():
    with open('data#/number_10.txt', 'r') as sys.stdin:
        for line in sys.stdin:
            if 'Exit' == line.rstrip():
               break
    output = 0
    # checks if there are the right number of arguments
    try:
        # converts the first two arguments to integers
        in_arg = int(line)
    except: # (argc != 2)
        # raises an error
        raise Exception("Incorrect arguments.\nUsage: python time_print.py [NUM]\ne.g.\npython time_print.py 23")

    # gets the start time for the loop
    start_time = time.time()
    # iterates over all numbers up to the input
    for i in range(in_arg):
        with open('data#/print2.txt', 'a') as sys.stdout:
            # prints the index
            print("{}, ".format(i), end="")
    # gets the end time for the loop
    end_time = time.time()

    # gets the total time
    run_time = end_time - start_time
    # prints the output

    with open('data#/print2.txt', 'a') as sys.stdout:
    	print("\n\nTime for loop: {} seconds\n".format(run_time))

if __name__ == "__main__":
    main()