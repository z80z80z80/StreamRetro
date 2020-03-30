# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/orell/Projekte/StreamRetro

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/orell/Projekte/StreamRetro

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/orell/Projekte/StreamRetro/CMakeFiles /home/orell/Projekte/StreamRetro/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/orell/Projekte/StreamRetro/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named streamRetro

# Build rule for target.
streamRetro: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 streamRetro
.PHONY : streamRetro

# fast build rule for target.
streamRetro/fast:
	$(MAKE) -f CMakeFiles/streamRetro.dir/build.make CMakeFiles/streamRetro.dir/build
.PHONY : streamRetro/fast

source/Core.o: source/Core.cpp.o

.PHONY : source/Core.o

# target to build an object file
source/Core.cpp.o:
	$(MAKE) -f CMakeFiles/streamRetro.dir/build.make CMakeFiles/streamRetro.dir/source/Core.cpp.o
.PHONY : source/Core.cpp.o

source/Core.i: source/Core.cpp.i

.PHONY : source/Core.i

# target to preprocess a source file
source/Core.cpp.i:
	$(MAKE) -f CMakeFiles/streamRetro.dir/build.make CMakeFiles/streamRetro.dir/source/Core.cpp.i
.PHONY : source/Core.cpp.i

source/Core.s: source/Core.cpp.s

.PHONY : source/Core.s

# target to generate assembly for a file
source/Core.cpp.s:
	$(MAKE) -f CMakeFiles/streamRetro.dir/build.make CMakeFiles/streamRetro.dir/source/Core.cpp.s
.PHONY : source/Core.cpp.s

source/LibRetroGameWrapper.o: source/LibRetroGameWrapper.cpp.o

.PHONY : source/LibRetroGameWrapper.o

# target to build an object file
source/LibRetroGameWrapper.cpp.o:
	$(MAKE) -f CMakeFiles/streamRetro.dir/build.make CMakeFiles/streamRetro.dir/source/LibRetroGameWrapper.cpp.o
.PHONY : source/LibRetroGameWrapper.cpp.o

source/LibRetroGameWrapper.i: source/LibRetroGameWrapper.cpp.i

.PHONY : source/LibRetroGameWrapper.i

# target to preprocess a source file
source/LibRetroGameWrapper.cpp.i:
	$(MAKE) -f CMakeFiles/streamRetro.dir/build.make CMakeFiles/streamRetro.dir/source/LibRetroGameWrapper.cpp.i
.PHONY : source/LibRetroGameWrapper.cpp.i

source/LibRetroGameWrapper.s: source/LibRetroGameWrapper.cpp.s

.PHONY : source/LibRetroGameWrapper.s

# target to generate assembly for a file
source/LibRetroGameWrapper.cpp.s:
	$(MAKE) -f CMakeFiles/streamRetro.dir/build.make CMakeFiles/streamRetro.dir/source/LibRetroGameWrapper.cpp.s
.PHONY : source/LibRetroGameWrapper.cpp.s

source/StreamRetroGame.o: source/StreamRetroGame.cpp.o

.PHONY : source/StreamRetroGame.o

# target to build an object file
source/StreamRetroGame.cpp.o:
	$(MAKE) -f CMakeFiles/streamRetro.dir/build.make CMakeFiles/streamRetro.dir/source/StreamRetroGame.cpp.o
.PHONY : source/StreamRetroGame.cpp.o

source/StreamRetroGame.i: source/StreamRetroGame.cpp.i

.PHONY : source/StreamRetroGame.i

# target to preprocess a source file
source/StreamRetroGame.cpp.i:
	$(MAKE) -f CMakeFiles/streamRetro.dir/build.make CMakeFiles/streamRetro.dir/source/StreamRetroGame.cpp.i
.PHONY : source/StreamRetroGame.cpp.i

source/StreamRetroGame.s: source/StreamRetroGame.cpp.s

.PHONY : source/StreamRetroGame.s

# target to generate assembly for a file
source/StreamRetroGame.cpp.s:
	$(MAKE) -f CMakeFiles/streamRetro.dir/build.make CMakeFiles/streamRetro.dir/source/StreamRetroGame.cpp.s
.PHONY : source/StreamRetroGame.cpp.s

source/main.o: source/main.cpp.o

.PHONY : source/main.o

# target to build an object file
source/main.cpp.o:
	$(MAKE) -f CMakeFiles/streamRetro.dir/build.make CMakeFiles/streamRetro.dir/source/main.cpp.o
.PHONY : source/main.cpp.o

source/main.i: source/main.cpp.i

.PHONY : source/main.i

# target to preprocess a source file
source/main.cpp.i:
	$(MAKE) -f CMakeFiles/streamRetro.dir/build.make CMakeFiles/streamRetro.dir/source/main.cpp.i
.PHONY : source/main.cpp.i

source/main.s: source/main.cpp.s

.PHONY : source/main.s

# target to generate assembly for a file
source/main.cpp.s:
	$(MAKE) -f CMakeFiles/streamRetro.dir/build.make CMakeFiles/streamRetro.dir/source/main.cpp.s
.PHONY : source/main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... streamRetro"
	@echo "... source/Core.o"
	@echo "... source/Core.i"
	@echo "... source/Core.s"
	@echo "... source/LibRetroGameWrapper.o"
	@echo "... source/LibRetroGameWrapper.i"
	@echo "... source/LibRetroGameWrapper.s"
	@echo "... source/StreamRetroGame.o"
	@echo "... source/StreamRetroGame.i"
	@echo "... source/StreamRetroGame.s"
	@echo "... source/main.o"
	@echo "... source/main.i"
	@echo "... source/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

