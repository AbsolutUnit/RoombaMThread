# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/anookabara/Desktop/assignment3-112710107/A3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/anookabara/Desktop/assignment3-112710107/A3

# Include any dependencies generated for this target.
include Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/compiler_depend.make

# Include the progress variables for this target.
include Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/progress.make

# Include the compile flags for this target's objects.
include Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/flags.make

Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/Algo_112628028.cpp.o: Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/flags.make
Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/Algo_112628028.cpp.o: Algorithm_1_112628028/Algo_112628028.cpp
Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/Algo_112628028.cpp.o: Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/anookabara/Desktop/assignment3-112710107/A3/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/Algo_112628028.cpp.o"
	cd /home/anookabara/Desktop/assignment3-112710107/A3/Algorithm_1_112628028 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/Algo_112628028.cpp.o -MF CMakeFiles/Algorithm_1_112628028.dir/Algo_112628028.cpp.o.d -o CMakeFiles/Algorithm_1_112628028.dir/Algo_112628028.cpp.o -c /home/anookabara/Desktop/assignment3-112710107/A3/Algorithm_1_112628028/Algo_112628028.cpp

Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/Algo_112628028.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Algorithm_1_112628028.dir/Algo_112628028.cpp.i"
	cd /home/anookabara/Desktop/assignment3-112710107/A3/Algorithm_1_112628028 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/anookabara/Desktop/assignment3-112710107/A3/Algorithm_1_112628028/Algo_112628028.cpp > CMakeFiles/Algorithm_1_112628028.dir/Algo_112628028.cpp.i

Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/Algo_112628028.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Algorithm_1_112628028.dir/Algo_112628028.cpp.s"
	cd /home/anookabara/Desktop/assignment3-112710107/A3/Algorithm_1_112628028 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/anookabara/Desktop/assignment3-112710107/A3/Algorithm_1_112628028/Algo_112628028.cpp -o CMakeFiles/Algorithm_1_112628028.dir/Algo_112628028.cpp.s

# Object files for target Algorithm_1_112628028
Algorithm_1_112628028_OBJECTS = \
"CMakeFiles/Algorithm_1_112628028.dir/Algo_112628028.cpp.o"

# External object files for target Algorithm_1_112628028
Algorithm_1_112628028_EXTERNAL_OBJECTS =

Algorithm_1_112628028/libAlgorithm_1_112628028.so: Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/Algo_112628028.cpp.o
Algorithm_1_112628028/libAlgorithm_1_112628028.so: Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/build.make
Algorithm_1_112628028/libAlgorithm_1_112628028.so: Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/anookabara/Desktop/assignment3-112710107/A3/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libAlgorithm_1_112628028.so"
	cd /home/anookabara/Desktop/assignment3-112710107/A3/Algorithm_1_112628028 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Algorithm_1_112628028.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/build: Algorithm_1_112628028/libAlgorithm_1_112628028.so
.PHONY : Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/build

Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/clean:
	cd /home/anookabara/Desktop/assignment3-112710107/A3/Algorithm_1_112628028 && $(CMAKE_COMMAND) -P CMakeFiles/Algorithm_1_112628028.dir/cmake_clean.cmake
.PHONY : Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/clean

Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/depend:
	cd /home/anookabara/Desktop/assignment3-112710107/A3 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/anookabara/Desktop/assignment3-112710107/A3 /home/anookabara/Desktop/assignment3-112710107/A3/Algorithm_1_112628028 /home/anookabara/Desktop/assignment3-112710107/A3 /home/anookabara/Desktop/assignment3-112710107/A3/Algorithm_1_112628028 /home/anookabara/Desktop/assignment3-112710107/A3/Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Algorithm_1_112628028/CMakeFiles/Algorithm_1_112628028.dir/depend

