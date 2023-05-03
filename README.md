[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/jEcQeig-)
# Aneek Barua and Qamber Jafri (Undergraduate team)

### Input Format
- House Name 
- MaxSteps (The maximum amount of steps the simulation can run for)
- MaxBattery (The maximum amount of battery in steps)
- Rows (The number of rows in the house)
- Cols (The number of columns in the house)
- The House Map
To run the code, enter the following after utilizing `cmake .` and `make all`:
```
./Simulator/Simulation -house_path{house file name}  -algo_path={algo path name} -num_threads={number of threads to use}-summary_only -display
```
House path should be a string for the path to a folder containing house files, which should have suffix '.house'. This is optional, as the Simulation will use the current directory if no house path is given.
Algo path should be a string for the path to a folder containing compiled Algorithm projects, or .so files. This is optional, as the Simulation will look in current directory if no algo path is given.
The display flag in this case dictates whether the Simulation runs quietly, or if it prints a map with the robot's location
and other pertinent information per iteration. This flag is optional, as it does not display if not needed.
The summary_only flag dictates whether or not to give normal output files, or just summary.csv. This flag is also optional, leaving it out will give all output files.
The num_threads flag dictates how many threads to run the Simulation on. This flag is optional, as it will set to default if not given. The default value is 10.

Algo_112628028:
This algorithm works like the game of snake, utilizing a NS bias and going all the way until a wall is hit, then going East or West. This algorithm, like the other one, utilizes a dp array to keep track of shortest path to dock from any point, and keeps track of visited/unvisited.

Algo_112710107:
This algorithm works similar to BFS, but utilizes Euclidean distance. It aims to go in concentric circles around the dock and chooses a direction to go that is unvisited and has the shortest Euclidean distance. This one also utilizes the same dp array and visited/unvisited caching.

Simulator:
Our aim here was to read in all the algorithms and load in all the houses to an array to read from them. During our run function, since both House and Algorithm have cached values that depend per simulation, we created a copy since we cannot assume an Algorithm has an implemented reset method. On each copy, we ran our simulation via the same methodology as before.

By separating the house information into a different class, we effectively removed a restriction from Simulation being dependent on either algo or house. We also used thread_local storage for pertinent Simulator member variables to allow each thread to run their own simulation with their own independent stack.

The ThreadPool implementation is a simple threaded queue for tasks that is pushed to by run to queue up tasks. This simply assigns each thread a task to complete and we await the joining of all threads to complete and generate summary.