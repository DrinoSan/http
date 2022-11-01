# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.24.0/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.24.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/sandi/DEV/http

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/sandi/DEV/http

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/opt/homebrew/Cellar/cmake/3.24.0/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/opt/homebrew/Cellar/cmake/3.24.0/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/sandi/DEV/http/CMakeFiles /Users/sandi/DEV/http//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/sandi/DEV/http/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named simpleHttpServer2

# Build rule for target.
simpleHttpServer2: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 simpleHttpServer2
.PHONY : simpleHttpServer2

# fast build rule for target.
simpleHttpServer2/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/build
.PHONY : simpleHttpServer2/fast

HttpMessage.o: HttpMessage.cpp.o
.PHONY : HttpMessage.o

# target to build an object file
HttpMessage.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/HttpMessage.cpp.o
.PHONY : HttpMessage.cpp.o

HttpMessage.i: HttpMessage.cpp.i
.PHONY : HttpMessage.i

# target to preprocess a source file
HttpMessage.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/HttpMessage.cpp.i
.PHONY : HttpMessage.cpp.i

HttpMessage.s: HttpMessage.cpp.s
.PHONY : HttpMessage.s

# target to generate assembly for a file
HttpMessage.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/HttpMessage.cpp.s
.PHONY : HttpMessage.cpp.s

HttpParser.o: HttpParser.cpp.o
.PHONY : HttpParser.o

# target to build an object file
HttpParser.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/HttpParser.cpp.o
.PHONY : HttpParser.cpp.o

HttpParser.i: HttpParser.cpp.i
.PHONY : HttpParser.i

# target to preprocess a source file
HttpParser.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/HttpParser.cpp.i
.PHONY : HttpParser.cpp.i

HttpParser.s: HttpParser.cpp.s
.PHONY : HttpParser.s

# target to generate assembly for a file
HttpParser.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/HttpParser.cpp.s
.PHONY : HttpParser.cpp.s

HttpRequest.o: HttpRequest.cpp.o
.PHONY : HttpRequest.o

# target to build an object file
HttpRequest.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/HttpRequest.cpp.o
.PHONY : HttpRequest.cpp.o

HttpRequest.i: HttpRequest.cpp.i
.PHONY : HttpRequest.i

# target to preprocess a source file
HttpRequest.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/HttpRequest.cpp.i
.PHONY : HttpRequest.cpp.i

HttpRequest.s: HttpRequest.cpp.s
.PHONY : HttpRequest.s

# target to generate assembly for a file
HttpRequest.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/HttpRequest.cpp.s
.PHONY : HttpRequest.cpp.s

HttpResponse.o: HttpResponse.cpp.o
.PHONY : HttpResponse.o

# target to build an object file
HttpResponse.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/HttpResponse.cpp.o
.PHONY : HttpResponse.cpp.o

HttpResponse.i: HttpResponse.cpp.i
.PHONY : HttpResponse.i

# target to preprocess a source file
HttpResponse.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/HttpResponse.cpp.i
.PHONY : HttpResponse.cpp.i

HttpResponse.s: HttpResponse.cpp.s
.PHONY : HttpResponse.s

# target to generate assembly for a file
HttpResponse.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/HttpResponse.cpp.s
.PHONY : HttpResponse.cpp.s

SimpleHttpServer.o: SimpleHttpServer.cpp.o
.PHONY : SimpleHttpServer.o

# target to build an object file
SimpleHttpServer.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/SimpleHttpServer.cpp.o
.PHONY : SimpleHttpServer.cpp.o

SimpleHttpServer.i: SimpleHttpServer.cpp.i
.PHONY : SimpleHttpServer.i

# target to preprocess a source file
SimpleHttpServer.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/SimpleHttpServer.cpp.i
.PHONY : SimpleHttpServer.cpp.i

SimpleHttpServer.s: SimpleHttpServer.cpp.s
.PHONY : SimpleHttpServer.s

# target to generate assembly for a file
SimpleHttpServer.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/SimpleHttpServer.cpp.s
.PHONY : SimpleHttpServer.cpp.s

main.o: main.cpp.o
.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i
.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s
.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/simpleHttpServer2.dir/build.make CMakeFiles/simpleHttpServer2.dir/main.cpp.s
.PHONY : main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... simpleHttpServer2"
	@echo "... HttpMessage.o"
	@echo "... HttpMessage.i"
	@echo "... HttpMessage.s"
	@echo "... HttpParser.o"
	@echo "... HttpParser.i"
	@echo "... HttpParser.s"
	@echo "... HttpRequest.o"
	@echo "... HttpRequest.i"
	@echo "... HttpRequest.s"
	@echo "... HttpResponse.o"
	@echo "... HttpResponse.i"
	@echo "... HttpResponse.s"
	@echo "... SimpleHttpServer.o"
	@echo "... SimpleHttpServer.i"
	@echo "... SimpleHttpServer.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

