# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = "F:\clion2022\CLion 2022.1.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "F:\clion2022\CLion 2022.1.3\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = F:\C_Compiler\Part1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = F:\C_Compiler\Part1\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Part1.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Part1.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Part1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Part1.dir/flags.make

CMakeFiles/Part1.dir/main.cpp.obj: CMakeFiles/Part1.dir/flags.make
CMakeFiles/Part1.dir/main.cpp.obj: ../main.cpp
CMakeFiles/Part1.dir/main.cpp.obj: CMakeFiles/Part1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=F:\C_Compiler\Part1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Part1.dir/main.cpp.obj"
	F:\CLION2~1\CLION2~1.3\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Part1.dir/main.cpp.obj -MF CMakeFiles\Part1.dir\main.cpp.obj.d -o CMakeFiles\Part1.dir\main.cpp.obj -c F:\C_Compiler\Part1\main.cpp

CMakeFiles/Part1.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Part1.dir/main.cpp.i"
	F:\CLION2~1\CLION2~1.3\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E F:\C_Compiler\Part1\main.cpp > CMakeFiles\Part1.dir\main.cpp.i

CMakeFiles/Part1.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Part1.dir/main.cpp.s"
	F:\CLION2~1\CLION2~1.3\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S F:\C_Compiler\Part1\main.cpp -o CMakeFiles\Part1.dir\main.cpp.s

# Object files for target Part1
Part1_OBJECTS = \
"CMakeFiles/Part1.dir/main.cpp.obj"

# External object files for target Part1
Part1_EXTERNAL_OBJECTS =

Part1.exe: CMakeFiles/Part1.dir/main.cpp.obj
Part1.exe: CMakeFiles/Part1.dir/build.make
Part1.exe: CMakeFiles/Part1.dir/linklibs.rsp
Part1.exe: CMakeFiles/Part1.dir/objects1.rsp
Part1.exe: CMakeFiles/Part1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=F:\C_Compiler\Part1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Part1.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Part1.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Part1.dir/build: Part1.exe
.PHONY : CMakeFiles/Part1.dir/build

CMakeFiles/Part1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Part1.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Part1.dir/clean

CMakeFiles/Part1.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" F:\C_Compiler\Part1 F:\C_Compiler\Part1 F:\C_Compiler\Part1\cmake-build-debug F:\C_Compiler\Part1\cmake-build-debug F:\C_Compiler\Part1\cmake-build-debug\CMakeFiles\Part1.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Part1.dir/depend

