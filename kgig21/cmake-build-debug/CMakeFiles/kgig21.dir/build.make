# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = "/Users/arsenijkarpov/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/192.6603.37/CLion.app/Contents/bin/cmake/mac/bin/cmake"

# The command to remove a file.
RM = "/Users/arsenijkarpov/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/192.6603.37/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/arsenijkarpov/2семестр/Компьютернаягеометрия/kgig21

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/arsenijkarpov/2семестр/Компьютернаягеометрия/kgig21/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/kgig21.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/kgig21.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/kgig21.dir/flags.make

CMakeFiles/kgig21.dir/main.cpp.o: CMakeFiles/kgig21.dir/flags.make
CMakeFiles/kgig21.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/arsenijkarpov/2семестр/Компьютернаягеометрия/kgig21/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/kgig21.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/kgig21.dir/main.cpp.o -c /Users/arsenijkarpov/2семестр/Компьютернаягеометрия/kgig21/main.cpp

CMakeFiles/kgig21.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kgig21.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/arsenijkarpov/2семестр/Компьютернаягеометрия/kgig21/main.cpp > CMakeFiles/kgig21.dir/main.cpp.i

CMakeFiles/kgig21.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kgig21.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/arsenijkarpov/2семестр/Компьютернаягеометрия/kgig21/main.cpp -o CMakeFiles/kgig21.dir/main.cpp.s

# Object files for target kgig21
kgig21_OBJECTS = \
"CMakeFiles/kgig21.dir/main.cpp.o"

# External object files for target kgig21
kgig21_EXTERNAL_OBJECTS =

kgig21: CMakeFiles/kgig21.dir/main.cpp.o
kgig21: CMakeFiles/kgig21.dir/build.make
kgig21: CMakeFiles/kgig21.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/arsenijkarpov/2семестр/Компьютернаягеометрия/kgig21/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable kgig21"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kgig21.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/kgig21.dir/build: kgig21

.PHONY : CMakeFiles/kgig21.dir/build

CMakeFiles/kgig21.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/kgig21.dir/cmake_clean.cmake
.PHONY : CMakeFiles/kgig21.dir/clean

CMakeFiles/kgig21.dir/depend:
	cd /Users/arsenijkarpov/2семестр/Компьютернаягеометрия/kgig21/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/arsenijkarpov/2семестр/Компьютернаягеометрия/kgig21 /Users/arsenijkarpov/2семестр/Компьютернаягеометрия/kgig21 /Users/arsenijkarpov/2семестр/Компьютернаягеометрия/kgig21/cmake-build-debug /Users/arsenijkarpov/2семестр/Компьютернаягеометрия/kgig21/cmake-build-debug /Users/arsenijkarpov/2семестр/Компьютернаягеометрия/kgig21/cmake-build-debug/CMakeFiles/kgig21.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/kgig21.dir/depend

