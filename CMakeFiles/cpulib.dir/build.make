# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/isaachamann/Repositories/bare_gb/build

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/isaachamann/Repositories/bare_gb

# Include any dependencies generated for this target.
include CMakeFiles/cpulib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/cpulib.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/cpulib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cpulib.dir/flags.make

CMakeFiles/cpulib.dir/src/src/memory.c.o: CMakeFiles/cpulib.dir/flags.make
CMakeFiles/cpulib.dir/src/src/memory.c.o: src/src/memory.c
CMakeFiles/cpulib.dir/src/src/memory.c.o: CMakeFiles/cpulib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/isaachamann/Repositories/bare_gb/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/cpulib.dir/src/src/memory.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/cpulib.dir/src/src/memory.c.o -MF CMakeFiles/cpulib.dir/src/src/memory.c.o.d -o CMakeFiles/cpulib.dir/src/src/memory.c.o -c /Users/isaachamann/Repositories/bare_gb/src/src/memory.c

CMakeFiles/cpulib.dir/src/src/memory.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cpulib.dir/src/src/memory.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/isaachamann/Repositories/bare_gb/src/src/memory.c > CMakeFiles/cpulib.dir/src/src/memory.c.i

CMakeFiles/cpulib.dir/src/src/memory.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cpulib.dir/src/src/memory.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/isaachamann/Repositories/bare_gb/src/src/memory.c -o CMakeFiles/cpulib.dir/src/src/memory.c.s

CMakeFiles/cpulib.dir/src/src/cpu.c.o: CMakeFiles/cpulib.dir/flags.make
CMakeFiles/cpulib.dir/src/src/cpu.c.o: src/src/cpu.c
CMakeFiles/cpulib.dir/src/src/cpu.c.o: CMakeFiles/cpulib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/isaachamann/Repositories/bare_gb/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/cpulib.dir/src/src/cpu.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/cpulib.dir/src/src/cpu.c.o -MF CMakeFiles/cpulib.dir/src/src/cpu.c.o.d -o CMakeFiles/cpulib.dir/src/src/cpu.c.o -c /Users/isaachamann/Repositories/bare_gb/src/src/cpu.c

CMakeFiles/cpulib.dir/src/src/cpu.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cpulib.dir/src/src/cpu.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/isaachamann/Repositories/bare_gb/src/src/cpu.c > CMakeFiles/cpulib.dir/src/src/cpu.c.i

CMakeFiles/cpulib.dir/src/src/cpu.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cpulib.dir/src/src/cpu.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/isaachamann/Repositories/bare_gb/src/src/cpu.c -o CMakeFiles/cpulib.dir/src/src/cpu.c.s

CMakeFiles/cpulib.dir/src/src/ppu.c.o: CMakeFiles/cpulib.dir/flags.make
CMakeFiles/cpulib.dir/src/src/ppu.c.o: src/src/ppu.c
CMakeFiles/cpulib.dir/src/src/ppu.c.o: CMakeFiles/cpulib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/isaachamann/Repositories/bare_gb/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/cpulib.dir/src/src/ppu.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/cpulib.dir/src/src/ppu.c.o -MF CMakeFiles/cpulib.dir/src/src/ppu.c.o.d -o CMakeFiles/cpulib.dir/src/src/ppu.c.o -c /Users/isaachamann/Repositories/bare_gb/src/src/ppu.c

CMakeFiles/cpulib.dir/src/src/ppu.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cpulib.dir/src/src/ppu.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/isaachamann/Repositories/bare_gb/src/src/ppu.c > CMakeFiles/cpulib.dir/src/src/ppu.c.i

CMakeFiles/cpulib.dir/src/src/ppu.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cpulib.dir/src/src/ppu.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/isaachamann/Repositories/bare_gb/src/src/ppu.c -o CMakeFiles/cpulib.dir/src/src/ppu.c.s

# Object files for target cpulib
cpulib_OBJECTS = \
"CMakeFiles/cpulib.dir/src/src/memory.c.o" \
"CMakeFiles/cpulib.dir/src/src/cpu.c.o" \
"CMakeFiles/cpulib.dir/src/src/ppu.c.o"

# External object files for target cpulib
cpulib_EXTERNAL_OBJECTS =

libcpulib.dylib: CMakeFiles/cpulib.dir/src/src/memory.c.o
libcpulib.dylib: CMakeFiles/cpulib.dir/src/src/cpu.c.o
libcpulib.dylib: CMakeFiles/cpulib.dir/src/src/ppu.c.o
libcpulib.dylib: CMakeFiles/cpulib.dir/build.make
libcpulib.dylib: CMakeFiles/cpulib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/isaachamann/Repositories/bare_gb/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C shared library libcpulib.dylib"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cpulib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cpulib.dir/build: libcpulib.dylib
.PHONY : CMakeFiles/cpulib.dir/build

CMakeFiles/cpulib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cpulib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cpulib.dir/clean

CMakeFiles/cpulib.dir/depend:
	cd /Users/isaachamann/Repositories/bare_gb && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/isaachamann/Repositories/bare_gb/build /Users/isaachamann/Repositories/bare_gb/build /Users/isaachamann/Repositories/bare_gb /Users/isaachamann/Repositories/bare_gb /Users/isaachamann/Repositories/bare_gb/CMakeFiles/cpulib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cpulib.dir/depend

