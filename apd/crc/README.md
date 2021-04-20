## TOC
1. Folder structure
2. Build instructions
3. Run unit tests
4. Implemented features


## 1. Folder structure

This C++ code implements CRC calculation routines that is used inside End-To-End protection module.
* Folders `include` and `src` contain source files of the *CRC* library.
* Folder `doc` contains documentation for the library.
* Folder `files` contain additional files  for the library, e.g. `pkg-config` files.


## 2. Build instructions

To build and install the *CRC* library execute the following commands:

    $ PREFIX=/usr/local
    $ mkdir <build-folder>
    $ cd <build-folder>
    $ cmake -DCMAKE_PREFIX_PATH=$PREFIX <source-folder>
    $ make
    $ make install

The following libraries will be built and installed during this process:
* *CRC* library will be installed under `$PREFIX/lib`.
* The headers files coresponding to the *CRC* library will be installed under `$PREFIX/include`.
* `pkg-config` file will be installed to `$PREFIX/share/pkgconfig`.
* Library unit tests will be installed to `$PREFIX/test`.


## 3. Run unit tests

To run unit tests build *CRC* library first.
Issue following command to run unit tests:

    $ cd <build-folder>
    $ make test


## 4. Implemented features

* Calculation of CRC8, CRC16, CRC32 and CRC64.
