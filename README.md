# Shared Memory Parallel Programming with Threads
For this part of the project, you will:
1. Create (in C or C++) a sequential program to solve a simulation problem. 
2. Create a parallel version of the program using pthreads or C++ threads. 
3. Write up results as described below for this part into an overall report (must be named 
report.pdf), and submit your code in your single submission zip (must be named 
hw5.zip) file under a directory called rainfall/. Extracting your hw5.zip file should extract 
report.pdf and the rainfall/ code directory into the current directory. Follow these 
naming conventions.  

Please refer to the course notes and examples, as well as the pthreads user documentation or 
man pages for help on compiling with pthreads as well as creating, managing, and 
synchronizing threads that have possibly conflicting reads and writes to shared memory.

### Rainfall Simulation  

The following is a description of the rainfall simulation problem that your program will solve. In 
this problem, we have a 2-dimensional landscape, which is essentially an NxN grid of points. 
Each point has an integer elevation associated with it that your program will read from an input 
file. In the rainfall simulation, drops of rain will fall onto the points of the 2D landscape. As those 
rain drops fall, the water will move in two ways: (1) rain drops will absorb into the ground at a 
point at a specified rate and (2) rain drops will trickle from a point in the landscape to their 
neighboring point(s) (north/south/east/west) that have the lowest elevation. Your program will 
perform time step simulation, simulating what happens across the 2D landscape in each time 
step as rain drops fall onto the points, trickle to lower elevation points, and get absorbed into the 
ground.  

Here are some more specific characteristics of the problem. Within a timestep of the rainfall 
simulation program, there are 3 things that can happen in the following sequence: (1) a drop of 
rain may fall onto a point in the landscape, (2) if there exists any rain drops on a point at a given 
timestep, some amount will be absorbed into the ground, and (3) if there remains any rain drops 
on a point at a given timestep, some amount will trickle away to the neighboring point(s) with the 
lowest elevation.
