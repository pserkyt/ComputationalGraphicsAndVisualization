Final Project:
--------------

Southern New Hampshire University
4/18/2021
by Cameron Winningham (aka Pserkyt) 

Controls:
---------

Mouse - Look
Scroll - Speed +/-
W - Forward
S - Backward
A - Strafe left
D - Strafe right
Q - Move up
E - Move down
P - Toggle Ortho/Perspective
LSHIFT - Run


Windows:
--------

Comes with precompiled binary located in the bin directory.
All dependencies are contained within the project directory.
Simply open cs330.sln in Visual Studio 2019 to compile.

Mac OSX:
--------

Download and install CMake. (https://cmake.org/)

Mac comes with OpenGL and DL libraries installed.
GLM and STB files included.

Macports: (recommended - everything installed to /opt/local)
$ sudo port install glfw
$ sudo port install glew

Homebrew: (alternative)
$ brew install glfw
$ brew install glew

Command line tools: (if not installed)
$ xcode-select --install

Open with CMake, configure, generate, and build.

Linux:
------

Currently UNTESTED! But it should work :)

Install dependencies for Debian-based systems:
$ sudo apt install build-essential cmake
$ sudo apt install libglfw3-dev libglew-dev

Compiling with CMake:
$ mkdir build
$ cd build
$ cmake ../ -G "Unix Makefiles"
$ make

Compiling with CMake GUI:
$ sudo apt install cmake-qt-gui

Open with CMake, configure, generate, and build.

Enjoy :)
