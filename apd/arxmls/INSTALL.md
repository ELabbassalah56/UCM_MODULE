## Building *ara-arxmls*

### Using Yocto to build QEMU image (*BitBake*)

To build the image using an already configured *BitBake* environment, issue this command:

    $ source poky/oe-init-build-env
    build$ bitbake core-image-apd-minimal

## Building using *AUTOSAR SDK*

In order to use the AUTOSAR SDK you first need to source it:

    $ source ~/autosar_sdk/environment-setup-i586-poky-linux

To build the ara-arxmls using the AUTOSAR SDK perform the following steps:

    $ cd ~/autosar_sources/ara-api/arxmls
    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_INSTALL_PREFIX=$OECORE_TARGET_SYSROOT/usr/ ..
    $ make install

