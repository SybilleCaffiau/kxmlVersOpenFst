# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/sybillecaffiau/Documents/PUBLICATIONS/ENCOURS/IHM2016/kxmlVersOpenFst

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/sybillecaffiau/Documents/PUBLICATIONS/ENCOURS/IHM2016/kxmlVersOpenFst

# Include any dependencies generated for this target.
include CMakeFiles/ProductionMdT.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ProductionMdT.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ProductionMdT.dir/flags.make

CMakeFiles/ProductionMdT.dir/parserKXML.cpp.o: CMakeFiles/ProductionMdT.dir/flags.make
CMakeFiles/ProductionMdT.dir/parserKXML.cpp.o: parserKXML.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sybillecaffiau/Documents/PUBLICATIONS/ENCOURS/IHM2016/kxmlVersOpenFst/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ProductionMdT.dir/parserKXML.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ProductionMdT.dir/parserKXML.cpp.o -c /Users/sybillecaffiau/Documents/PUBLICATIONS/ENCOURS/IHM2016/kxmlVersOpenFst/parserKXML.cpp

CMakeFiles/ProductionMdT.dir/parserKXML.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ProductionMdT.dir/parserKXML.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sybillecaffiau/Documents/PUBLICATIONS/ENCOURS/IHM2016/kxmlVersOpenFst/parserKXML.cpp > CMakeFiles/ProductionMdT.dir/parserKXML.cpp.i

CMakeFiles/ProductionMdT.dir/parserKXML.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ProductionMdT.dir/parserKXML.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sybillecaffiau/Documents/PUBLICATIONS/ENCOURS/IHM2016/kxmlVersOpenFst/parserKXML.cpp -o CMakeFiles/ProductionMdT.dir/parserKXML.cpp.s

CMakeFiles/ProductionMdT.dir/parserKXML.cpp.o.requires:

.PHONY : CMakeFiles/ProductionMdT.dir/parserKXML.cpp.o.requires

CMakeFiles/ProductionMdT.dir/parserKXML.cpp.o.provides: CMakeFiles/ProductionMdT.dir/parserKXML.cpp.o.requires
	$(MAKE) -f CMakeFiles/ProductionMdT.dir/build.make CMakeFiles/ProductionMdT.dir/parserKXML.cpp.o.provides.build
.PHONY : CMakeFiles/ProductionMdT.dir/parserKXML.cpp.o.provides

CMakeFiles/ProductionMdT.dir/parserKXML.cpp.o.provides.build: CMakeFiles/ProductionMdT.dir/parserKXML.cpp.o


# Object files for target ProductionMdT
ProductionMdT_OBJECTS = \
"CMakeFiles/ProductionMdT.dir/parserKXML.cpp.o"

# External object files for target ProductionMdT
ProductionMdT_EXTERNAL_OBJECTS =

ProductionMdT: CMakeFiles/ProductionMdT.dir/parserKXML.cpp.o
ProductionMdT: CMakeFiles/ProductionMdT.dir/build.make
ProductionMdT: libtinyxml.a
ProductionMdT: CMakeFiles/ProductionMdT.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/sybillecaffiau/Documents/PUBLICATIONS/ENCOURS/IHM2016/kxmlVersOpenFst/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ProductionMdT"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ProductionMdT.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ProductionMdT.dir/build: ProductionMdT

.PHONY : CMakeFiles/ProductionMdT.dir/build

CMakeFiles/ProductionMdT.dir/requires: CMakeFiles/ProductionMdT.dir/parserKXML.cpp.o.requires

.PHONY : CMakeFiles/ProductionMdT.dir/requires

CMakeFiles/ProductionMdT.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ProductionMdT.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ProductionMdT.dir/clean

CMakeFiles/ProductionMdT.dir/depend:
	cd /Users/sybillecaffiau/Documents/PUBLICATIONS/ENCOURS/IHM2016/kxmlVersOpenFst && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/sybillecaffiau/Documents/PUBLICATIONS/ENCOURS/IHM2016/kxmlVersOpenFst /Users/sybillecaffiau/Documents/PUBLICATIONS/ENCOURS/IHM2016/kxmlVersOpenFst /Users/sybillecaffiau/Documents/PUBLICATIONS/ENCOURS/IHM2016/kxmlVersOpenFst /Users/sybillecaffiau/Documents/PUBLICATIONS/ENCOURS/IHM2016/kxmlVersOpenFst /Users/sybillecaffiau/Documents/PUBLICATIONS/ENCOURS/IHM2016/kxmlVersOpenFst/CMakeFiles/ProductionMdT.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ProductionMdT.dir/depend
