# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.19

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Users\Pikachu08181998\CLion 2019.3.5\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Users\Pikachu08181998\CLion 2019.3.5\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "D:\Pikachu08181998\Documents\ece4100\New folder\Project3"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "D:\Pikachu08181998\Documents\ece4100\New folder\Project3\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/Project3.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Project3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Project3.dir/flags.make

CMakeFiles/Project3.dir/procsim.cpp.obj: CMakeFiles/Project3.dir/flags.make
CMakeFiles/Project3.dir/procsim.cpp.obj: CMakeFiles/Project3.dir/includes_CXX.rsp
CMakeFiles/Project3.dir/procsim.cpp.obj: ../procsim.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\Pikachu08181998\Documents\ece4100\New folder\Project3\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Project3.dir/procsim.cpp.obj"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Project3.dir\procsim.cpp.obj -c "D:\Pikachu08181998\Documents\ece4100\New folder\Project3\procsim.cpp"

CMakeFiles/Project3.dir/procsim.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Project3.dir/procsim.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\Pikachu08181998\Documents\ece4100\New folder\Project3\procsim.cpp" > CMakeFiles\Project3.dir\procsim.cpp.i

CMakeFiles/Project3.dir/procsim.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Project3.dir/procsim.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\Pikachu08181998\Documents\ece4100\New folder\Project3\procsim.cpp" -o CMakeFiles\Project3.dir\procsim.cpp.s

CMakeFiles/Project3.dir/procsim_driver.cpp.obj: CMakeFiles/Project3.dir/flags.make
CMakeFiles/Project3.dir/procsim_driver.cpp.obj: CMakeFiles/Project3.dir/includes_CXX.rsp
CMakeFiles/Project3.dir/procsim_driver.cpp.obj: ../procsim_driver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\Pikachu08181998\Documents\ece4100\New folder\Project3\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Project3.dir/procsim_driver.cpp.obj"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Project3.dir\procsim_driver.cpp.obj -c "D:\Pikachu08181998\Documents\ece4100\New folder\Project3\procsim_driver.cpp"

CMakeFiles/Project3.dir/procsim_driver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Project3.dir/procsim_driver.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\Pikachu08181998\Documents\ece4100\New folder\Project3\procsim_driver.cpp" > CMakeFiles\Project3.dir\procsim_driver.cpp.i

CMakeFiles/Project3.dir/procsim_driver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Project3.dir/procsim_driver.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\Pikachu08181998\Documents\ece4100\New folder\Project3\procsim_driver.cpp" -o CMakeFiles\Project3.dir\procsim_driver.cpp.s

# Object files for target Project3
Project3_OBJECTS = \
"CMakeFiles/Project3.dir/procsim.cpp.obj" \
"CMakeFiles/Project3.dir/procsim_driver.cpp.obj"

# External object files for target Project3
Project3_EXTERNAL_OBJECTS =

Project3.exe: CMakeFiles/Project3.dir/procsim.cpp.obj
Project3.exe: CMakeFiles/Project3.dir/procsim_driver.cpp.obj
Project3.exe: CMakeFiles/Project3.dir/build.make
Project3.exe: CMakeFiles/Project3.dir/linklibs.rsp
Project3.exe: CMakeFiles/Project3.dir/objects1.rsp
Project3.exe: CMakeFiles/Project3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="D:\Pikachu08181998\Documents\ece4100\New folder\Project3\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable Project3.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Project3.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Project3.dir/build: Project3.exe

.PHONY : CMakeFiles/Project3.dir/build

CMakeFiles/Project3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Project3.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Project3.dir/clean

CMakeFiles/Project3.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "D:\Pikachu08181998\Documents\ece4100\New folder\Project3" "D:\Pikachu08181998\Documents\ece4100\New folder\Project3" "D:\Pikachu08181998\Documents\ece4100\New folder\Project3\cmake-build-debug" "D:\Pikachu08181998\Documents\ece4100\New folder\Project3\cmake-build-debug" "D:\Pikachu08181998\Documents\ece4100\New folder\Project3\cmake-build-debug\CMakeFiles\Project3.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Project3.dir/depend

