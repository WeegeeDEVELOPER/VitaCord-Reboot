# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/bashe/Desktop/vita_projects/VitaCord_reboot

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/bashe/Desktop/vita_projects/VitaCord_reboot

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/c/Users/bashe/Desktop/vita_projects/VitaCord_reboot/CMakeFiles /mnt/c/Users/bashe/Desktop/vita_projects/VitaCord_reboot/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/c/Users/bashe/Desktop/vita_projects/VitaCord_reboot/CMakeFiles 0
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
# Target rules for targets named VitaCord_Reboot.vpk

# Build rule for target.
VitaCord_Reboot.vpk: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 VitaCord_Reboot.vpk
.PHONY : VitaCord_Reboot.vpk

# fast build rule for target.
VitaCord_Reboot.vpk/fast:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.vpk.dir/build.make CMakeFiles/VitaCord_Reboot.vpk.dir/build
.PHONY : VitaCord_Reboot.vpk/fast

#=============================================================================
# Target rules for targets named VitaCord_Reboot.self

# Build rule for target.
VitaCord_Reboot.self: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 VitaCord_Reboot.self
.PHONY : VitaCord_Reboot.self

# fast build rule for target.
VitaCord_Reboot.self/fast:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.self.dir/build.make CMakeFiles/VitaCord_Reboot.self.dir/build
.PHONY : VitaCord_Reboot.self/fast

#=============================================================================
# Target rules for targets named VitaCord_Reboot.elf

# Build rule for target.
VitaCord_Reboot.elf: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 VitaCord_Reboot.elf
.PHONY : VitaCord_Reboot.elf

# fast build rule for target.
VitaCord_Reboot.elf/fast:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/build
.PHONY : VitaCord_Reboot.elf/fast

src/Discord.obj: src/Discord.cpp.obj

.PHONY : src/Discord.obj

# target to build an object file
src/Discord.cpp.obj:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/Discord.cpp.obj
.PHONY : src/Discord.cpp.obj

src/Discord.i: src/Discord.cpp.i

.PHONY : src/Discord.i

# target to preprocess a source file
src/Discord.cpp.i:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/Discord.cpp.i
.PHONY : src/Discord.cpp.i

src/Discord.s: src/Discord.cpp.s

.PHONY : src/Discord.s

# target to generate assembly for a file
src/Discord.cpp.s:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/Discord.cpp.s
.PHONY : src/Discord.cpp.s

src/DiscordApp.obj: src/DiscordApp.cpp.obj

.PHONY : src/DiscordApp.obj

# target to build an object file
src/DiscordApp.cpp.obj:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/DiscordApp.cpp.obj
.PHONY : src/DiscordApp.cpp.obj

src/DiscordApp.i: src/DiscordApp.cpp.i

.PHONY : src/DiscordApp.i

# target to preprocess a source file
src/DiscordApp.cpp.i:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/DiscordApp.cpp.i
.PHONY : src/DiscordApp.cpp.i

src/DiscordApp.s: src/DiscordApp.cpp.s

.PHONY : src/DiscordApp.s

# target to generate assembly for a file
src/DiscordApp.cpp.s:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/DiscordApp.cpp.s
.PHONY : src/DiscordApp.cpp.s

src/VitaGUI.obj: src/VitaGUI.cpp.obj

.PHONY : src/VitaGUI.obj

# target to build an object file
src/VitaGUI.cpp.obj:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/VitaGUI.cpp.obj
.PHONY : src/VitaGUI.cpp.obj

src/VitaGUI.i: src/VitaGUI.cpp.i

.PHONY : src/VitaGUI.i

# target to preprocess a source file
src/VitaGUI.cpp.i:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/VitaGUI.cpp.i
.PHONY : src/VitaGUI.cpp.i

src/VitaGUI.s: src/VitaGUI.cpp.s

.PHONY : src/VitaGUI.s

# target to generate assembly for a file
src/VitaGUI.cpp.s:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/VitaGUI.cpp.s
.PHONY : src/VitaGUI.cpp.s

src/VitaIME.obj: src/VitaIME.cpp.obj

.PHONY : src/VitaIME.obj

# target to build an object file
src/VitaIME.cpp.obj:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/VitaIME.cpp.obj
.PHONY : src/VitaIME.cpp.obj

src/VitaIME.i: src/VitaIME.cpp.i

.PHONY : src/VitaIME.i

# target to preprocess a source file
src/VitaIME.cpp.i:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/VitaIME.cpp.i
.PHONY : src/VitaIME.cpp.i

src/VitaIME.s: src/VitaIME.cpp.s

.PHONY : src/VitaIME.s

# target to generate assembly for a file
src/VitaIME.cpp.s:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/VitaIME.cpp.s
.PHONY : src/VitaIME.cpp.s

src/VitaNet.obj: src/VitaNet.cpp.obj

.PHONY : src/VitaNet.obj

# target to build an object file
src/VitaNet.cpp.obj:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/VitaNet.cpp.obj
.PHONY : src/VitaNet.cpp.obj

src/VitaNet.i: src/VitaNet.cpp.i

.PHONY : src/VitaNet.i

# target to preprocess a source file
src/VitaNet.cpp.i:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/VitaNet.cpp.i
.PHONY : src/VitaNet.cpp.i

src/VitaNet.s: src/VitaNet.cpp.s

.PHONY : src/VitaNet.s

# target to generate assembly for a file
src/VitaNet.cpp.s:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/VitaNet.cpp.s
.PHONY : src/VitaNet.cpp.s

src/VitaPad.obj: src/VitaPad.cpp.obj

.PHONY : src/VitaPad.obj

# target to build an object file
src/VitaPad.cpp.obj:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/VitaPad.cpp.obj
.PHONY : src/VitaPad.cpp.obj

src/VitaPad.i: src/VitaPad.cpp.i

.PHONY : src/VitaPad.i

# target to preprocess a source file
src/VitaPad.cpp.i:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/VitaPad.cpp.i
.PHONY : src/VitaPad.cpp.i

src/VitaPad.s: src/VitaPad.cpp.s

.PHONY : src/VitaPad.s

# target to generate assembly for a file
src/VitaPad.cpp.s:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/VitaPad.cpp.s
.PHONY : src/VitaPad.cpp.s

src/VitaTouch.obj: src/VitaTouch.cpp.obj

.PHONY : src/VitaTouch.obj

# target to build an object file
src/VitaTouch.cpp.obj:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/VitaTouch.cpp.obj
.PHONY : src/VitaTouch.cpp.obj

src/VitaTouch.i: src/VitaTouch.cpp.i

.PHONY : src/VitaTouch.i

# target to preprocess a source file
src/VitaTouch.cpp.i:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/VitaTouch.cpp.i
.PHONY : src/VitaTouch.cpp.i

src/VitaTouch.s: src/VitaTouch.cpp.s

.PHONY : src/VitaTouch.s

# target to generate assembly for a file
src/VitaTouch.cpp.s:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/VitaTouch.cpp.s
.PHONY : src/VitaTouch.cpp.s

src/easyencryptor.obj: src/easyencryptor.cpp.obj

.PHONY : src/easyencryptor.obj

# target to build an object file
src/easyencryptor.cpp.obj:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/easyencryptor.cpp.obj
.PHONY : src/easyencryptor.cpp.obj

src/easyencryptor.i: src/easyencryptor.cpp.i

.PHONY : src/easyencryptor.i

# target to preprocess a source file
src/easyencryptor.cpp.i:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/easyencryptor.cpp.i
.PHONY : src/easyencryptor.cpp.i

src/easyencryptor.s: src/easyencryptor.cpp.s

.PHONY : src/easyencryptor.s

# target to generate assembly for a file
src/easyencryptor.cpp.s:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/easyencryptor.cpp.s
.PHONY : src/easyencryptor.cpp.s

src/log.obj: src/log.cpp.obj

.PHONY : src/log.obj

# target to build an object file
src/log.cpp.obj:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/log.cpp.obj
.PHONY : src/log.cpp.obj

src/log.i: src/log.cpp.i

.PHONY : src/log.i

# target to preprocess a source file
src/log.cpp.i:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/log.cpp.i
.PHONY : src/log.cpp.i

src/log.s: src/log.cpp.s

.PHONY : src/log.s

# target to generate assembly for a file
src/log.cpp.s:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/log.cpp.s
.PHONY : src/log.cpp.s

src/main.obj: src/main.cpp.obj

.PHONY : src/main.obj

# target to build an object file
src/main.cpp.obj:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/main.cpp.obj
.PHONY : src/main.cpp.obj

src/main.i: src/main.cpp.i

.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s

.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) -f CMakeFiles/VitaCord_Reboot.elf.dir/build.make CMakeFiles/VitaCord_Reboot.elf.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... VitaCord_Reboot.vpk"
	@echo "... VitaCord_Reboot.self"
	@echo "... VitaCord_Reboot.elf"
	@echo "... src/Discord.obj"
	@echo "... src/Discord.i"
	@echo "... src/Discord.s"
	@echo "... src/DiscordApp.obj"
	@echo "... src/DiscordApp.i"
	@echo "... src/DiscordApp.s"
	@echo "... src/VitaGUI.obj"
	@echo "... src/VitaGUI.i"
	@echo "... src/VitaGUI.s"
	@echo "... src/VitaIME.obj"
	@echo "... src/VitaIME.i"
	@echo "... src/VitaIME.s"
	@echo "... src/VitaNet.obj"
	@echo "... src/VitaNet.i"
	@echo "... src/VitaNet.s"
	@echo "... src/VitaPad.obj"
	@echo "... src/VitaPad.i"
	@echo "... src/VitaPad.s"
	@echo "... src/VitaTouch.obj"
	@echo "... src/VitaTouch.i"
	@echo "... src/VitaTouch.s"
	@echo "... src/easyencryptor.obj"
	@echo "... src/easyencryptor.i"
	@echo "... src/easyencryptor.s"
	@echo "... src/log.obj"
	@echo "... src/log.i"
	@echo "... src/log.s"
	@echo "... src/main.obj"
	@echo "... src/main.i"
	@echo "... src/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

