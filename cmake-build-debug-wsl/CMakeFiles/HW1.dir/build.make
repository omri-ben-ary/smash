# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/User/Desktop/OS/smash

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/User/Desktop/OS/smash/cmake-build-debug-wsl

# Include any dependencies generated for this target.
include CMakeFiles/HW1.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/HW1.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/HW1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HW1.dir/flags.make

CMakeFiles/HW1.dir/Commands.cpp.o: CMakeFiles/HW1.dir/flags.make
CMakeFiles/HW1.dir/Commands.cpp.o: ../Commands.cpp
CMakeFiles/HW1.dir/Commands.cpp.o: CMakeFiles/HW1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/User/Desktop/OS/smash/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/HW1.dir/Commands.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HW1.dir/Commands.cpp.o -MF CMakeFiles/HW1.dir/Commands.cpp.o.d -o CMakeFiles/HW1.dir/Commands.cpp.o -c /mnt/c/Users/User/Desktop/OS/smash/Commands.cpp

CMakeFiles/HW1.dir/Commands.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HW1.dir/Commands.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/User/Desktop/OS/smash/Commands.cpp > CMakeFiles/HW1.dir/Commands.cpp.i

CMakeFiles/HW1.dir/Commands.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HW1.dir/Commands.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/User/Desktop/OS/smash/Commands.cpp -o CMakeFiles/HW1.dir/Commands.cpp.s

CMakeFiles/HW1.dir/signals.cpp.o: CMakeFiles/HW1.dir/flags.make
CMakeFiles/HW1.dir/signals.cpp.o: ../signals.cpp
CMakeFiles/HW1.dir/signals.cpp.o: CMakeFiles/HW1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/User/Desktop/OS/smash/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/HW1.dir/signals.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HW1.dir/signals.cpp.o -MF CMakeFiles/HW1.dir/signals.cpp.o.d -o CMakeFiles/HW1.dir/signals.cpp.o -c /mnt/c/Users/User/Desktop/OS/smash/signals.cpp

CMakeFiles/HW1.dir/signals.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HW1.dir/signals.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/User/Desktop/OS/smash/signals.cpp > CMakeFiles/HW1.dir/signals.cpp.i

CMakeFiles/HW1.dir/signals.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HW1.dir/signals.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/User/Desktop/OS/smash/signals.cpp -o CMakeFiles/HW1.dir/signals.cpp.s

CMakeFiles/HW1.dir/smash.cpp.o: CMakeFiles/HW1.dir/flags.make
CMakeFiles/HW1.dir/smash.cpp.o: ../smash.cpp
CMakeFiles/HW1.dir/smash.cpp.o: CMakeFiles/HW1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/User/Desktop/OS/smash/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/HW1.dir/smash.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HW1.dir/smash.cpp.o -MF CMakeFiles/HW1.dir/smash.cpp.o.d -o CMakeFiles/HW1.dir/smash.cpp.o -c /mnt/c/Users/User/Desktop/OS/smash/smash.cpp

CMakeFiles/HW1.dir/smash.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HW1.dir/smash.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/User/Desktop/OS/smash/smash.cpp > CMakeFiles/HW1.dir/smash.cpp.i

CMakeFiles/HW1.dir/smash.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HW1.dir/smash.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/User/Desktop/OS/smash/smash.cpp -o CMakeFiles/HW1.dir/smash.cpp.s

CMakeFiles/HW1.dir/printStatements.cpp.o: CMakeFiles/HW1.dir/flags.make
CMakeFiles/HW1.dir/printStatements.cpp.o: ../printStatements.cpp
CMakeFiles/HW1.dir/printStatements.cpp.o: CMakeFiles/HW1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/User/Desktop/OS/smash/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/HW1.dir/printStatements.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HW1.dir/printStatements.cpp.o -MF CMakeFiles/HW1.dir/printStatements.cpp.o.d -o CMakeFiles/HW1.dir/printStatements.cpp.o -c /mnt/c/Users/User/Desktop/OS/smash/printStatements.cpp

CMakeFiles/HW1.dir/printStatements.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HW1.dir/printStatements.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/User/Desktop/OS/smash/printStatements.cpp > CMakeFiles/HW1.dir/printStatements.cpp.i

CMakeFiles/HW1.dir/printStatements.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HW1.dir/printStatements.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/User/Desktop/OS/smash/printStatements.cpp -o CMakeFiles/HW1.dir/printStatements.cpp.s

# Object files for target HW1
HW1_OBJECTS = \
"CMakeFiles/HW1.dir/Commands.cpp.o" \
"CMakeFiles/HW1.dir/signals.cpp.o" \
"CMakeFiles/HW1.dir/smash.cpp.o" \
"CMakeFiles/HW1.dir/printStatements.cpp.o"

# External object files for target HW1
HW1_EXTERNAL_OBJECTS =

HW1: CMakeFiles/HW1.dir/Commands.cpp.o
HW1: CMakeFiles/HW1.dir/signals.cpp.o
HW1: CMakeFiles/HW1.dir/smash.cpp.o
HW1: CMakeFiles/HW1.dir/printStatements.cpp.o
HW1: CMakeFiles/HW1.dir/build.make
HW1: CMakeFiles/HW1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/User/Desktop/OS/smash/cmake-build-debug-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable HW1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HW1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HW1.dir/build: HW1
.PHONY : CMakeFiles/HW1.dir/build

CMakeFiles/HW1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HW1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HW1.dir/clean

CMakeFiles/HW1.dir/depend:
	cd /mnt/c/Users/User/Desktop/OS/smash/cmake-build-debug-wsl && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/User/Desktop/OS/smash /mnt/c/Users/User/Desktop/OS/smash /mnt/c/Users/User/Desktop/OS/smash/cmake-build-debug-wsl /mnt/c/Users/User/Desktop/OS/smash/cmake-build-debug-wsl /mnt/c/Users/User/Desktop/OS/smash/cmake-build-debug-wsl/CMakeFiles/HW1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/HW1.dir/depend

