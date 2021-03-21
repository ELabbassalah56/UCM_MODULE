# Building *Update and Configuration Management*

## Using Yocto to build QEMU image (*BitBake*)

To build the image using an already configured *BitBake* environment, issue this command:

    $ source poky/oe-init-build-env
    build$ bitbake core-image-apd-minimal

## Building using *AUTOSAR SDK*

In order to use the AUTOSAR SDK you first need to source it:

    $ source ~/autosar_sdk/environment-setup-i586-poky-linux

To build and install the ucm arxmls using the AUTOSAR SDK perform the following steps:

    $ cd ~/autosar_sources/ara-api/ucm/arxmls
    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_INSTALL_PREFIX=$OECORE_NATIVE_SYSROOT/usr/ ..
    $ make install

To build the PackageManager using the AUTOSAR SDK perform the following steps:

    $ cd ~/autosar_sources/ara-api/ucm/pkgmgr
    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_INSTALL_PREFIX=$OECORE_TARGET_SYSROOT/usr/ ..
    $ make

The unit tests executable is created by default (ARA_ENABLE_TESTS=ON). To be able to run unit tests in docker perform the following steps:

    $ cd ~/autosar_sources/ara-api/ucm/pkgmgr
    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_INSTALL_PREFIX=$OECORE_TARGET_SYSROOT/usr/ -DARA_RUN_TESTS=ON ..
    $ make
    $ make dock
    $ make test
    $ make undock

For detailed and colored output, add -DARA_TEST_ARGS="--gtest_color=yes" to cmake call:
    $ cmake -DCMAKE_INSTALL_PREFIX=$OECORE_TARGET_SYSROOT/usr/ -DARA_RUN_TESTS=ON -DARA_TEST_ARGS="--gtest_color=yes" ..
Then call start test with:
    $ make ARGS="-V" test

You may need to import the yocto image first using 'docker import /path/to/poky/build/tmp/deploy/images/qemux86/core-image-apd-devel-qemux86.tar.bz2 ara-apd:test' as is described in the wiki (https://wiki.autosar.org/doku.php?id=ap_dev_guide:guidelines:unit_testing)

In order to build the documentation for the PackageManager you can enable the doxygen target. The documentation will be generated into your build/doc folder.

    $ cd ~/autosar_sources/ara-api/ucm/pkgmgr
    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_INSTALL_PREFIX=$OECORE_TARGET_SYSROOT/usr/ -DARA_ENABLE_DOXYGEN=ON ..
    $ make documentation

Optionally, a pdf document can be generated:
    $ cd doc/latex
    $ make pdf

Note that the unit tests expect test data to execute properly. The testdata located at pkgmgr/test/data/ is expected at the path /usr/share/apdtest/ucm/.
We recommend to execute unit tests in docker or in qemu, especially because unit tests copy, move or recursively delete directories.
