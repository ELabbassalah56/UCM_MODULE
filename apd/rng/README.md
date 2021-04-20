TOC
===================
1. Folder structure
2. Build instructions
3. Implemented features


1. Folder structure
===================

This C++ code intends to provide a common really simple random number generator:
* Folder `include` contains header files of the RNG lib.
* Folder `src` contains source files of the RNG lib.

2. Build instructions
===================

To build and install the rng lib execute the following commands:

    $ source /path/to/environment-setup-i586-poky-linux
    $ mkdir -p /path/to/build
    $ cd /path/to/build
    $ cmake -DCMAKE_INSTALL_PREFIX=$OECORE_TARGET_SYSROOT/usr/ /path/to/ara-api/apd/rng
    $ make
    $ make install

The following libraries will be built and installed during this process:
- *apd::rng* library will be installed under `/usr/local/libs`.
- The headers file coresponding to the *apd::rng* library will be installed under `/usr/local/include`.
- *apd-rngConfig.cmake* will be installed to `usr/lib/cmake/apd-rng`.


3. Implemented features
===================

- Simple random number generation
- Based on uniform distribution
- For now only returns int values between two bounds
