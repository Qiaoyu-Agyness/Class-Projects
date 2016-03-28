Brian Rubenstein and Qiaoyu Liao
Written by Brian Rubenstein
10/3/2014
Version 1
Program 4
CS 2303
Programming Language: C++

Contribution:
Brian Rubenstein: 60%
Qiaoyu Liao: 40%


Problem Description:
This program needs to take in an arbitrary of robots to deliver items in the RoboMall. These robots arrive with a delay of 25 
time units per, and move through the mall dropping off their time units at one time unit per item they drop off at each 
store. After they have dropped off all of their items, storing them in a tree of all items contained in the mall, they 
leave. After all the robots have left the shoppers come in. There are an arbitrary amount of shoppers, each one having
an arrival time and an arbitrary amount of items, each with a name and number the shopper is looking for. When the shoppers
enter the mall, they need to build a list of stores on the fly with the items they are looking for. If a store does not have
the items they need, they delete that item from the list. They then travel from store to store, buying their items. If there
are multiple shoppers at one store, if the store is on the first floor they deal with it in a first come first serve pattern,
if it is on the second floor, they deal with it in a round robin pattern. Once they finish buying all their items they leave
the RoboMall.

Overall Design:
This program uses an eventList structure to deal with concurrency. Robots/shoppers are stored in event lists, differently for
each type (robots and shoppers have their own individual event lists). The event lists are stored as classes, and the objects
are stored as structures. 
The program is structured with two header files and 10 C++ source files

Program discrepancies with problem description:
The program prints out when it obtains an item when it begins processing its final time: so when a FCFS starts or when
the final time slice for an item happens.

Program issues:
Some shoppers get dropped before they can finish/leave the robomall. We could not figure out why this is happening.

Program assumptions:
The only assumption of the program is that it will recieve its input data properly.

Build the program using: 
make


Run the program with a call to the executable and a numerical argument for timeslice.
./main timeSlice