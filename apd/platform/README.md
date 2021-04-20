TOC
===================
1. Folder structure
2. Build instructions
3. Implemented features


1. Folder structure
===================

This C++ code intends to show several generic features implementation for demonstrator:
* Folders `include` and `src` contain source files of the libplatform library.
* Folder `cmake` folder contains the library *CMake* config file.
* Folder `doc` contains the documentation of all implemented classes.


2. Build instructions
===================

Prerequisites:
* *CMake* installed on the system
* *gcc*/*g++* installed on the system

To build and install the Execution Management API execute the following commands:

    $ mkdir <build-folder>
    $ cd <build-folder>
    $ cmake <source-folder>
    $ make
    $ make install

The following libraries will be built and installed during this process:
- *libplatform* library will be installed under `/usr/local/libs`.
- The headers file coresponding to the *libplatform* library will be installed under `/usr/local/include`.


3. Implemented features
===================

- Basic low level pipe communication
- Generic implementation of server/client pipe based communication
- Various generic utilities
