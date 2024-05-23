# SENG265-Software-Development-Methods
UVIC SENG 265 2023

The following are projects from SENG 265


# Assignment 1: 
Introduction to C programming. A C program that receives an ics calendar file and display the events in the format:

{month} {day}, {Year}

--------------------- (length of dashes must match length of date above)

{start time} to {end time}: {event description} {{Event location}}


example:

May 19, 2023

------------ `

10:30 AM to 11:30 AM: meeting {{Home}}

11:30 AM to 12:30 PM: lunch {{Starbucks}}

June 01, 2023

------------- `

11:15 AM to 12:30 PM: Coffee {{Starbucks}}

End of example

Thoughts: This was my first contact with C. in hindsight using structs and time.h among many other improvements would have made the assignment significantly easier.

# Assignment 2: 
A python script to sort through "top songs" csv files by either "popularity", "danceability" or "energy" from highet to lowest using the PANDAS library. 

arguments are the .csv files, the number of entries to display and the type sorting type

ex `./music_manager.py --sortBy=danceability --display=3 --files=top_songs_2010.csv`

would outup:

artist,song,year,danceability

Rick Astley,Never Gonna Give You Up,1987,0.924

Darude,Sandstorm,2000,0.922

Beethoven,Ode to Joy,1822,0.88

Thoughts: We did something similar in lab so this was rather straightforward.

# Assignment 3

The same as assignment 2 but programmed in C and utilizing dynamic memory and a given linked list structure to complete the task.

Program parses through the text in the .csv file records the specified metrics in a linked list node and adds it to a list in order determined by the sortBy parameter.

The program then prints the contents of the linked list in the desired format.

Thoughts: Took some of the knowledge from the first assignment and applied it here. Was still not super familiar with C at this point.

# Assignment 4

A python script that draws a series of circles, rectangles and ellipses in random colors and sizes in an .html file

Example:

![a431](https://github.com/VICMEND/SENG265-Software-Development-Methods/assets/127559762/fc2c2fa4-57f7-4b0a-a82a-d9f1705723be)

Thoughts: Nothing too outstanding or notable about this one, Just coding.

