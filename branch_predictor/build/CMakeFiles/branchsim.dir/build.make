# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/d/Pikachu08181998/Documents/ece4100/Project2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/d/Pikachu08181998/Documents/ece4100/Project2/build

# Include any dependencies generated for this target.
include CMakeFiles/branchsim.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/branchsim.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/branchsim.dir/flags.make

CMakeFiles/branchsim.dir/src/branchsim_driver.cpp.o: CMakeFiles/branchsim.dir/flags.make
CMakeFiles/branchsim.dir/src/branchsim_driver.cpp.o: ../src/branchsim_driver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/Pikachu08181998/Documents/ece4100/Project2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/branchsim.dir/src/branchsim_driver.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/branchsim.dir/src/branchsim_driver.cpp.o -c /mnt/d/Pikachu08181998/Documents/ece4100/Project2/src/branchsim_driver.cpp

CMakeFiles/branchsim.dir/src/branchsim_driver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/branchsim.dir/src/branchsim_driver.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/Pikachu08181998/Documents/ece4100/Project2/src/branchsim_driver.cpp > CMakeFiles/branchsim.dir/src/branchsim_driver.cpp.i

CMakeFiles/branchsim.dir/src/branchsim_driver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/branchsim.dir/src/branchsim_driver.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/Pikachu08181998/Documents/ece4100/Project2/src/branchsim_driver.cpp -o CMakeFiles/branchsim.dir/src/branchsim_driver.cpp.s

CMakeFiles/branchsim.dir/src/branchsim.cpp.o: CMakeFiles/branchsim.dir/flags.make
CMakeFiles/branchsim.dir/src/branchsim.cpp.o: ../src/branchsim.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/Pikachu08181998/Documents/ece4100/Project2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/branchsim.dir/src/branchsim.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/branchsim.dir/src/branchsim.cpp.o -c /mnt/d/Pikachu08181998/Documents/ece4100/Project2/src/branchsim.cpp

CMakeFiles/branchsim.dir/src/branchsim.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/branchsim.dir/src/branchsim.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/Pikachu08181998/Documents/ece4100/Project2/src/branchsim.cpp > CMakeFiles/branchsim.dir/src/branchsim.cpp.i

CMakeFiles/branchsim.dir/src/branchsim.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/branchsim.dir/src/branchsim.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/Pikachu08181998/Documents/ece4100/Project2/src/branchsim.cpp -o CMakeFiles/branchsim.dir/src/branchsim.cpp.s

# Object files for target branchsim
branchsim_OBJECTS = \
"CMakeFiles/branchsim.dir/src/branchsim_driver.cpp.o" \
"CMakeFiles/branchsim.dir/src/branchsim.cpp.o"

# External object files for target branchsim
branchsim_EXTERNAL_OBJECTS =

branchsim: CMakeFiles/branchsim.dir/src/branchsim_driver.cpp.o
branchsim: CMakeFiles/branchsim.dir/src/branchsim.cpp.o
branchsim: CMakeFiles/branchsim.dir/build.make
branchsim: CMakeFiles/branchsim.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/d/Pikachu08181998/Documents/ece4100/Project2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable branchsim"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/branchsim.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/branchsim.dir/build: branchsim

.PHONY : CMakeFiles/branchsim.dir/build

CMakeFiles/branchsim.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/branchsim.dir/cmake_clean.cmake
.PHONY : CMakeFiles/branchsim.dir/clean

CMakeFiles/branchsim.dir/depend:
	cd /mnt/d/Pikachu08181998/Documents/ece4100/Project2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/Pikachu08181998/Documents/ece4100/Project2 /mnt/d/Pikachu08181998/Documents/ece4100/Project2 /mnt/d/Pikachu08181998/Documents/ece4100/Project2/build /mnt/d/Pikachu08181998/Documents/ece4100/Project2/build /mnt/d/Pikachu08181998/Documents/ece4100/Project2/build/CMakeFiles/branchsim.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/branchsim.dir/depend
