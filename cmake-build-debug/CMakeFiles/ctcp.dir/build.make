# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /home/cs551/Desktop/clion-2017.2.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/cs551/Desktop/clion-2017.2.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cs551/navaneethrvce-cs551/lab4/ctcp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cs551/navaneethrvce-cs551/lab4/ctcp/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ctcp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ctcp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ctcp.dir/flags.make

CMakeFiles/ctcp.dir/ctcp.c.o: CMakeFiles/ctcp.dir/flags.make
CMakeFiles/ctcp.dir/ctcp.c.o: ../ctcp.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cs551/navaneethrvce-cs551/lab4/ctcp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/ctcp.dir/ctcp.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ctcp.dir/ctcp.c.o   -c /home/cs551/navaneethrvce-cs551/lab4/ctcp/ctcp.c

CMakeFiles/ctcp.dir/ctcp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ctcp.dir/ctcp.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cs551/navaneethrvce-cs551/lab4/ctcp/ctcp.c > CMakeFiles/ctcp.dir/ctcp.c.i

CMakeFiles/ctcp.dir/ctcp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ctcp.dir/ctcp.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cs551/navaneethrvce-cs551/lab4/ctcp/ctcp.c -o CMakeFiles/ctcp.dir/ctcp.c.s

CMakeFiles/ctcp.dir/ctcp.c.o.requires:

.PHONY : CMakeFiles/ctcp.dir/ctcp.c.o.requires

CMakeFiles/ctcp.dir/ctcp.c.o.provides: CMakeFiles/ctcp.dir/ctcp.c.o.requires
	$(MAKE) -f CMakeFiles/ctcp.dir/build.make CMakeFiles/ctcp.dir/ctcp.c.o.provides.build
.PHONY : CMakeFiles/ctcp.dir/ctcp.c.o.provides

CMakeFiles/ctcp.dir/ctcp.c.o.provides.build: CMakeFiles/ctcp.dir/ctcp.c.o


CMakeFiles/ctcp.dir/ctcp_linked_list.c.o: CMakeFiles/ctcp.dir/flags.make
CMakeFiles/ctcp.dir/ctcp_linked_list.c.o: ../ctcp_linked_list.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cs551/navaneethrvce-cs551/lab4/ctcp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/ctcp.dir/ctcp_linked_list.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ctcp.dir/ctcp_linked_list.c.o   -c /home/cs551/navaneethrvce-cs551/lab4/ctcp/ctcp_linked_list.c

CMakeFiles/ctcp.dir/ctcp_linked_list.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ctcp.dir/ctcp_linked_list.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cs551/navaneethrvce-cs551/lab4/ctcp/ctcp_linked_list.c > CMakeFiles/ctcp.dir/ctcp_linked_list.c.i

CMakeFiles/ctcp.dir/ctcp_linked_list.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ctcp.dir/ctcp_linked_list.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cs551/navaneethrvce-cs551/lab4/ctcp/ctcp_linked_list.c -o CMakeFiles/ctcp.dir/ctcp_linked_list.c.s

CMakeFiles/ctcp.dir/ctcp_linked_list.c.o.requires:

.PHONY : CMakeFiles/ctcp.dir/ctcp_linked_list.c.o.requires

CMakeFiles/ctcp.dir/ctcp_linked_list.c.o.provides: CMakeFiles/ctcp.dir/ctcp_linked_list.c.o.requires
	$(MAKE) -f CMakeFiles/ctcp.dir/build.make CMakeFiles/ctcp.dir/ctcp_linked_list.c.o.provides.build
.PHONY : CMakeFiles/ctcp.dir/ctcp_linked_list.c.o.provides

CMakeFiles/ctcp.dir/ctcp_linked_list.c.o.provides.build: CMakeFiles/ctcp.dir/ctcp_linked_list.c.o


CMakeFiles/ctcp.dir/ctcp_sys_internal.c.o: CMakeFiles/ctcp.dir/flags.make
CMakeFiles/ctcp.dir/ctcp_sys_internal.c.o: ../ctcp_sys_internal.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cs551/navaneethrvce-cs551/lab4/ctcp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/ctcp.dir/ctcp_sys_internal.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ctcp.dir/ctcp_sys_internal.c.o   -c /home/cs551/navaneethrvce-cs551/lab4/ctcp/ctcp_sys_internal.c

CMakeFiles/ctcp.dir/ctcp_sys_internal.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ctcp.dir/ctcp_sys_internal.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cs551/navaneethrvce-cs551/lab4/ctcp/ctcp_sys_internal.c > CMakeFiles/ctcp.dir/ctcp_sys_internal.c.i

CMakeFiles/ctcp.dir/ctcp_sys_internal.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ctcp.dir/ctcp_sys_internal.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cs551/navaneethrvce-cs551/lab4/ctcp/ctcp_sys_internal.c -o CMakeFiles/ctcp.dir/ctcp_sys_internal.c.s

CMakeFiles/ctcp.dir/ctcp_sys_internal.c.o.requires:

.PHONY : CMakeFiles/ctcp.dir/ctcp_sys_internal.c.o.requires

CMakeFiles/ctcp.dir/ctcp_sys_internal.c.o.provides: CMakeFiles/ctcp.dir/ctcp_sys_internal.c.o.requires
	$(MAKE) -f CMakeFiles/ctcp.dir/build.make CMakeFiles/ctcp.dir/ctcp_sys_internal.c.o.provides.build
.PHONY : CMakeFiles/ctcp.dir/ctcp_sys_internal.c.o.provides

CMakeFiles/ctcp.dir/ctcp_sys_internal.c.o.provides.build: CMakeFiles/ctcp.dir/ctcp_sys_internal.c.o


CMakeFiles/ctcp.dir/ctcp_utils.c.o: CMakeFiles/ctcp.dir/flags.make
CMakeFiles/ctcp.dir/ctcp_utils.c.o: ../ctcp_utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cs551/navaneethrvce-cs551/lab4/ctcp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/ctcp.dir/ctcp_utils.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ctcp.dir/ctcp_utils.c.o   -c /home/cs551/navaneethrvce-cs551/lab4/ctcp/ctcp_utils.c

CMakeFiles/ctcp.dir/ctcp_utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ctcp.dir/ctcp_utils.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cs551/navaneethrvce-cs551/lab4/ctcp/ctcp_utils.c > CMakeFiles/ctcp.dir/ctcp_utils.c.i

CMakeFiles/ctcp.dir/ctcp_utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ctcp.dir/ctcp_utils.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cs551/navaneethrvce-cs551/lab4/ctcp/ctcp_utils.c -o CMakeFiles/ctcp.dir/ctcp_utils.c.s

CMakeFiles/ctcp.dir/ctcp_utils.c.o.requires:

.PHONY : CMakeFiles/ctcp.dir/ctcp_utils.c.o.requires

CMakeFiles/ctcp.dir/ctcp_utils.c.o.provides: CMakeFiles/ctcp.dir/ctcp_utils.c.o.requires
	$(MAKE) -f CMakeFiles/ctcp.dir/build.make CMakeFiles/ctcp.dir/ctcp_utils.c.o.provides.build
.PHONY : CMakeFiles/ctcp.dir/ctcp_utils.c.o.provides

CMakeFiles/ctcp.dir/ctcp_utils.c.o.provides.build: CMakeFiles/ctcp.dir/ctcp_utils.c.o


CMakeFiles/ctcp.dir/test.c.o: CMakeFiles/ctcp.dir/flags.make
CMakeFiles/ctcp.dir/test.c.o: ../test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cs551/navaneethrvce-cs551/lab4/ctcp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/ctcp.dir/test.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ctcp.dir/test.c.o   -c /home/cs551/navaneethrvce-cs551/lab4/ctcp/test.c

CMakeFiles/ctcp.dir/test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ctcp.dir/test.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cs551/navaneethrvce-cs551/lab4/ctcp/test.c > CMakeFiles/ctcp.dir/test.c.i

CMakeFiles/ctcp.dir/test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ctcp.dir/test.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cs551/navaneethrvce-cs551/lab4/ctcp/test.c -o CMakeFiles/ctcp.dir/test.c.s

CMakeFiles/ctcp.dir/test.c.o.requires:

.PHONY : CMakeFiles/ctcp.dir/test.c.o.requires

CMakeFiles/ctcp.dir/test.c.o.provides: CMakeFiles/ctcp.dir/test.c.o.requires
	$(MAKE) -f CMakeFiles/ctcp.dir/build.make CMakeFiles/ctcp.dir/test.c.o.provides.build
.PHONY : CMakeFiles/ctcp.dir/test.c.o.provides

CMakeFiles/ctcp.dir/test.c.o.provides.build: CMakeFiles/ctcp.dir/test.c.o


# Object files for target ctcp
ctcp_OBJECTS = \
"CMakeFiles/ctcp.dir/ctcp.c.o" \
"CMakeFiles/ctcp.dir/ctcp_linked_list.c.o" \
"CMakeFiles/ctcp.dir/ctcp_sys_internal.c.o" \
"CMakeFiles/ctcp.dir/ctcp_utils.c.o" \
"CMakeFiles/ctcp.dir/test.c.o"

# External object files for target ctcp
ctcp_EXTERNAL_OBJECTS =

ctcp: CMakeFiles/ctcp.dir/ctcp.c.o
ctcp: CMakeFiles/ctcp.dir/ctcp_linked_list.c.o
ctcp: CMakeFiles/ctcp.dir/ctcp_sys_internal.c.o
ctcp: CMakeFiles/ctcp.dir/ctcp_utils.c.o
ctcp: CMakeFiles/ctcp.dir/test.c.o
ctcp: CMakeFiles/ctcp.dir/build.make
ctcp: CMakeFiles/ctcp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cs551/navaneethrvce-cs551/lab4/ctcp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable ctcp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ctcp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ctcp.dir/build: ctcp

.PHONY : CMakeFiles/ctcp.dir/build

CMakeFiles/ctcp.dir/requires: CMakeFiles/ctcp.dir/ctcp.c.o.requires
CMakeFiles/ctcp.dir/requires: CMakeFiles/ctcp.dir/ctcp_linked_list.c.o.requires
CMakeFiles/ctcp.dir/requires: CMakeFiles/ctcp.dir/ctcp_sys_internal.c.o.requires
CMakeFiles/ctcp.dir/requires: CMakeFiles/ctcp.dir/ctcp_utils.c.o.requires
CMakeFiles/ctcp.dir/requires: CMakeFiles/ctcp.dir/test.c.o.requires

.PHONY : CMakeFiles/ctcp.dir/requires

CMakeFiles/ctcp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ctcp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ctcp.dir/clean

CMakeFiles/ctcp.dir/depend:
	cd /home/cs551/navaneethrvce-cs551/lab4/ctcp/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cs551/navaneethrvce-cs551/lab4/ctcp /home/cs551/navaneethrvce-cs551/lab4/ctcp /home/cs551/navaneethrvce-cs551/lab4/ctcp/cmake-build-debug /home/cs551/navaneethrvce-cs551/lab4/ctcp/cmake-build-debug /home/cs551/navaneethrvce-cs551/lab4/ctcp/cmake-build-debug/CMakeFiles/ctcp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ctcp.dir/depend

