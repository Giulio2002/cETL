# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /snap/cmake/619/bin/cmake

# The command to remove a file.
RM = /snap/cmake/619/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/giulio/silkworm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/giulio/silkworm

# Include any dependencies generated for this target.
include CMakeFiles/evmone.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/evmone.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/evmone.dir/flags.make

CMakeFiles/evmone.dir/evmone/lib/evmone/analysis.cpp.o: CMakeFiles/evmone.dir/flags.make
CMakeFiles/evmone.dir/evmone/lib/evmone/analysis.cpp.o: evmone/lib/evmone/analysis.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/giulio/silkworm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/evmone.dir/evmone/lib/evmone/analysis.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/evmone.dir/evmone/lib/evmone/analysis.cpp.o -c /home/giulio/silkworm/evmone/lib/evmone/analysis.cpp

CMakeFiles/evmone.dir/evmone/lib/evmone/analysis.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/evmone.dir/evmone/lib/evmone/analysis.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/giulio/silkworm/evmone/lib/evmone/analysis.cpp > CMakeFiles/evmone.dir/evmone/lib/evmone/analysis.cpp.i

CMakeFiles/evmone.dir/evmone/lib/evmone/analysis.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/evmone.dir/evmone/lib/evmone/analysis.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/giulio/silkworm/evmone/lib/evmone/analysis.cpp -o CMakeFiles/evmone.dir/evmone/lib/evmone/analysis.cpp.s

CMakeFiles/evmone.dir/evmone/lib/evmone/instructions.cpp.o: CMakeFiles/evmone.dir/flags.make
CMakeFiles/evmone.dir/evmone/lib/evmone/instructions.cpp.o: evmone/lib/evmone/instructions.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/giulio/silkworm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/evmone.dir/evmone/lib/evmone/instructions.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/evmone.dir/evmone/lib/evmone/instructions.cpp.o -c /home/giulio/silkworm/evmone/lib/evmone/instructions.cpp

CMakeFiles/evmone.dir/evmone/lib/evmone/instructions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/evmone.dir/evmone/lib/evmone/instructions.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/giulio/silkworm/evmone/lib/evmone/instructions.cpp > CMakeFiles/evmone.dir/evmone/lib/evmone/instructions.cpp.i

CMakeFiles/evmone.dir/evmone/lib/evmone/instructions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/evmone.dir/evmone/lib/evmone/instructions.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/giulio/silkworm/evmone/lib/evmone/instructions.cpp -o CMakeFiles/evmone.dir/evmone/lib/evmone/instructions.cpp.s

# Object files for target evmone
evmone_OBJECTS = \
"CMakeFiles/evmone.dir/evmone/lib/evmone/analysis.cpp.o" \
"CMakeFiles/evmone.dir/evmone/lib/evmone/instructions.cpp.o"

# External object files for target evmone
evmone_EXTERNAL_OBJECTS =

libevmone.a: CMakeFiles/evmone.dir/evmone/lib/evmone/analysis.cpp.o
libevmone.a: CMakeFiles/evmone.dir/evmone/lib/evmone/instructions.cpp.o
libevmone.a: CMakeFiles/evmone.dir/build.make
libevmone.a: CMakeFiles/evmone.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/giulio/silkworm/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libevmone.a"
	$(CMAKE_COMMAND) -P CMakeFiles/evmone.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/evmone.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/evmone.dir/build: libevmone.a

.PHONY : CMakeFiles/evmone.dir/build

CMakeFiles/evmone.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/evmone.dir/cmake_clean.cmake
.PHONY : CMakeFiles/evmone.dir/clean

CMakeFiles/evmone.dir/depend:
	cd /home/giulio/silkworm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/giulio/silkworm /home/giulio/silkworm /home/giulio/silkworm /home/giulio/silkworm /home/giulio/silkworm/CMakeFiles/evmone.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/evmone.dir/depend

