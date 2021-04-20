apd-cmake-modules
=================

This CMake project installs a new CMake module which provides for standardizing
the common settings which should be used by all CMake-buildable packages within
the Adaptive Platform Demonstrator code base.

The use of this module immediate advantages for the developer and APD

1. All functional clusters use a consistent set of compiler warnings and features.
2. These can be adjusted globally at the Yocto level, rather than in individual CMake scripts.
3. Much boiler-plate code is removed from conforming CMake scripts.

Usage
-----

Usage is straightforward, requiring changes in two places


### Top Level Cmake Script


The top-level CMake script should contain the following line

    include(AUTOSAR/apd)

Typically, this will occur after the mandatory `cmake_minimum_required` and
`project` commands. Making the opening lines of the top level CMake script
look like this

```
cmake_minimum_required(VERSION 3.4)
project(my-project VERSION 1.0.0 LANGUAGES CXX)
include(AUTOSAR/apd)
```

The module itself is installed directly into the `Modules` directory for the
version of `cmake` being used (typically `/usr/share/cmake-<major>.<minor>/Modules`)
and is thus available without modifying `CMAKE_MODULE_PATH`.

### Changes to Your Yocto Recipe


You recipe currently will include the following line:

    inherit cmake

Simply change this to 

    inherit apd_cmake

This will add a dependency on `apd-cmake-modules-native`, as well as setting some
global flags in `OECMAKE_EXTRA`.

Macros Defined
--------------

Currently, the module declares a single macro, `apd_defaults`. This is invoked
during the `include` process and requires no further action as it is required
for all packages.
 
Further macros will be added in the future and documented below.

### `apd_defaults()`


The `apd_defaults` macro enforces the following aspects of the CMake coding
guidelines.

* `CMAKE_BUILD_TYPE` is set correctly, defaulting to `Debug`
* Standard warnings are enabled for all C++ compilation.
* `CMAKE_CXX_EXTENSIONS` is set to `OFF`, consistent with specifying `-Wpedantic`
* The C++ Dialect is set to C++ 14

