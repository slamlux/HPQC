import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

import pandas as pd
import math as m
import random

from pathlib import Path
import sys

def generate_path(home_folder = str(Path.home()), subfolder = '/data/', basename = 'output', extension = 'txt'):
    """This function creates the path to store the data. Note that the data is not 
    stored in the directory the code is executed in. This prevents git repos 
    from becoming cluttered with data that should be separate.

    Keyword Args:
        home_folder (str): the root folder in which the file is specified
            Defaults to Home
        subfolder (str): the subfolder in which the file is specified
            Defaults to /data/
        basename (str): the base of the filename to be specified
            Defaults to output
        extension (str): the file type to be specified
            Defaults to txt
    """
    # uses the method Path.home() to find the home directory in any OS
    output_folder = home_folder + subfolder  # appends a subdirectory within it.
    filename = basename + '.' + extension  # defines the filename the output is to be saved in
    output_path = output_folder + filename  # creates the output path
    return output_path

def execute_time_step(data, rope, i):
    """This function carries out the update process for the animation.  
    All points on the string 'follow the leader', except for point 0, 
    which is 'driven' by in a simple harmonic oscillator

    Args:
        rope (list of Line2D): the plot showing the data to be updated
        y_positions (1-d NumPy array): the underlying data to be ploted
        time (float): the current time in the animation
    """
    # updates the positions
    x_positions, y_positions = extract_position(data, i=i)

    # updates the plotted data in the animation
    rope.set_data(x_positions, y_positions)
    return i


def reset_plots(rope, y_positions):
    """This function resets the data in the plot when the plotting 
    loop is completed.

    Args:
        rope (list of Line2D): the plot showing the data to be updated
        y_positions (1-d NumPy array): the underlying data to be ploted
    """
    # Calls the initial function to set up the string
    x_positions, y_positions = configure_rope()

    # sets the data for the rope to the initial value
    rope.set_data(x_positions, y_positions)


def animate(time, data, rope):
    """This function carries out the animation steps and resets the 
    plots at the start of the loop if allowed to run.

    Args:
        time (float): current timestamp in the animation
        rope (list of Line2D): the plot showing the data to be updated
        y_positions (1-d NumPy array): the underlying data to be ploted

    Returns:
        rope, (tuple of (lists of Line2D)): iterable collection of the 
            plots which have been animated in this function. 
            Note that even if only one plot is included in the animation, 
            this must still be returned as an iterable, speciied by a 
            trailing comma. 
    """
    # updates the positions
    x_positions, y_positions = extract_position(data, i=time)

    # updates the plotted data in the animation
    rope.set_data(x_positions, y_positions)
    
    # return a tuple of plots (lists of Line2D)
    return rope,


def configure_animation(frame_count = 125, fps = 25):
    """This function sets up the variables reauored for the animations in
    Python. These are returned as a tuple which must be parsed by the 
    calling process.

    Keyword Args:
        frame_count (number): Intended number of frames the animation will 
            will run for. After this time, the animation will loop
            Defaults to 125
        fps (number): Intended number of frames per second in the
            animation. This number is returned as well.
            Defaults to 25 

    Returns:
        times (np array of floats): the time stamps (in seconds) for each 
            step in the animation. In this program, these are directly 
            mapped to the actual time of the animation, but this is not 
            required by FuncAnimation().
        interval (int): the number of milliseconds between frames, to a 
            minimum of 1ms. Used to configure the animation. In this 
            program, this is matched to the framerate at which the 
            animation is saved, but this is not required by ani.save()
        fps (number): the number of frames per secdond to be used in the
            final version of the animation saved to file.
    """
    # calculates the total run time in the animation
    run_time = frame_count/fps 

    # calculates the interval in milliseconds between frames, minimum 1
    interval = max([int(1000 / fps), 1])

    # calculates the time step in seconds to be used between frames
    time_step = interval / 1000

    # creates a numpy array of the times in the animation
    times = np.linspace(0., run_time, int(frame_count))

    # returns a tuple with the times, interval and frames per second
    return times, interval, fps


def configure_rope(length=50, end_point=True, step_size=1, start = 0):
    """Configures the parameters of the string which is to be used in 
    the animation.

    Keyword Args:
        length (number): the length of the string to simulate
            Defaults to 50
        end_point (bool): whether to include a final point at 
            point_labels = length. 
            Defaults to True
        step_size (number): the distance between simulated points
            Defaults to 1
        start (number): the position of the start of the string.
            Defaults to 0

    Returns:
        x_positions (Numpy array of floats): initial values for the
            x-positions of the points on the string
        y_positions (Numpy array of floats): initial values for the 
            y-positions of the points on the string
    """
    # calculates the number of points to simulate
    points = int((length/step_size) + end_point)

    # calculates the position of the end of the string
    stop = start + length

    # calculates the x-positions of the points on the string
    x_positions = np.linspace(start=start, stop=stop, num=points, endpoint=end_point)

    # sets the initial positions of the simulated points to zero
    y_positions = np.zeros(points)

    # returns a tuple containing the positions of the points on the string
    return x_positions, y_positions


def configure_plot(x_positions, y_positions):
    """This function sets up the plot for the animation.  It sets the
    initial x and y values, the title and the limits for the axes.

    Args: 
        x_positions (Numpy array of floats): initial values for the
            x-positions of the points on the string
        y_positions (Numpy array of floats): initial values for the
            y-positions of the points on the string

    Returns:
        fig (matplotlib.figure.Figure): the figure to be animated
        rope (list of Line2D): the plot showing the data to be animated
    """
    # creates the figure and the axis to be plotted
    fig, (ax1) = plt.subplots(1, 1)

    # sets the overall title of the plot
    fig.suptitle('Simulation of Vibrations in a string')

    # sets the x and y limits to be plotted
    ax1.set_ylim(-1.1, 1.1)

    # creates the plot which will be animated
    rope, = ax1.plot(x_positions, y_positions, "o", markersize=5, color="green", label="points on string")

    # creates the legend entry
    ax1.legend(handles=[rope], bbox_to_anchor=(0, 0), loc='upper left')

    # returns the figure and the plot to be animated
    return fig, rope


def get_data(filename, other=2):
    """This function reads the data from the csv file and returns it,
    together with data properties which are used elsewhere in the 
    program. 

    Args:
        filename (string): the path to the file containing the data

    Keword Arguments:
        other (int): the number of columns in the dataframe that
            represent data other than the position measurements
            Defaults to 2

    Returns:
        data (Pandas Dataframe): a dataframe with rows for each point
            in time and a column for every point on the string that
            we are modelling
        num_positions (int): the number of x-positions on the line
            that are being modelled
        num_times (int): the number of samples of the positions in 
            the supplied data.  Will be used as frame count later.

    """
    # tries to read the data from the file into a pandas dataframe
    try:
        data = pd.read_csv(filename, skipinitialspace=True)
    # if that file is not found
    except FileNotFoundError:
        # explains what has gone wrong
        print("The file you have specified, {} does not exist. Have you given the correct path to the file?".format(filename))
        # exits with error status
        exit(-1)

    # calculates how many positions have been measured
    num_positions = len(data.columns)-other

    # calculates how many samples were taken
    num_times = len(data)

    # returns the data and meta data
    return data, num_positions, num_times


def extract_position(data, i=0, other=2):
    """This function extracts the position of the points on the string
    from a dataframe for a given row.  The user can specify the number
    of other columns that are present in the dataframe

    Args:
        data (Pandas Dataframe): a dataframe with rows for each point
            in time and a column for every point on the string that
            we are modelling

    Keyword Arguments:
        i (int): the index representing the row of the dataframe
            Defaults to 0
        other (int): the number of columns in the dataframe that 
            represent data other than the position measurements
            Defaults to 2
    """
    # converts a row of the table into a value that can be parsed
    row = data.iloc[i]

    # gets the y positions from elements other to end of the row
    # flattens this into a numpy array and flattens it to be 
    # suitable for use with the plotting routines
    y_positions = np.array(row[other:]).flatten()

    # makes the x positions from the length of the row
    x_positions = np.arange(len(row)-other)

    # returns the x and y positions
    return x_positions, y_positions


def get_file_name(extension="txt"):
    """This function reads the filename from the sytem arguments and 
    captures some of the most common errors.  It does not cover more
    subtle errors that a user might cause.
    Args:
        extension (string): a string containing the extension for the
            type of file that is sought

    Returns:
        filename (string): a string that should contain a file of 
            the type specified in the argument
    """
    # tries to read the filename from the system arguments
    try:
        filename = sys.argv[1]
    # if there aren't enough arguments
    except IndexError:
        # explain the error
        print("You must specify a file to plot.\nCorrect Useage: \n\t{} [FILENAME].{}".format(sys.argv[0], extension))
        # exit the program with error status
        exit(-1)

    # if the extension of the file is not csv
    if ((filename.split('.')[-1])!=extension):
        # explain the error
        print("The file you have specified, {} does not appear to be a {} file.".format(filename, extension))
        # exit the program with error status
        exit(-1)

    # in other circumstances, return the filename
    return filename


def main():
    """This is the main function that executes the rest of the program
    Using a main function instead of executing directly in the global 
    namespace allows for local variables and better control of scope.
    """
    # gets the filename from the command line
    filename = get_file_name("csv")

    # gets the data and its dimensions from the file
    data, num_positions, num_times = get_data(filename)

    # sets up the variables to manage the animation
    times, interval, fps = configure_animation(frame_count=num_times)

    # sets up the points on the string 
    x_positions, y_positions = extract_position(data)

    # configures the initial state of the plot, including x and y positions of points on the string
    fig, rope = configure_plot(x_positions, y_positions)

    # initialises the python animation 
    ani = animation.FuncAnimation(fig, animate, num_times, interval=interval, blit=True, # mandatory animation arguments
                                  fargs=(data, rope)) # arguments to the animate function

    # saves the animation to disk
    filename = generate_path(basename = sys.argv[2], extension = 'gif')
    ani.save(filename=filename, writer="pillow", fps=fps)


# we use this convention to ensure that if we import functions from this script, it is not executed
if __name__ == "__main__":
    main() # this is a good practice to get used to

