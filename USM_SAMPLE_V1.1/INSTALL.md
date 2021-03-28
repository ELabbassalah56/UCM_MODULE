## Building *UCM Examples*

### Using Yocto to build QEMU image (*BitBake*)

To build the image using an already configured *BitBake* environment, issue this command:

    $ source poky/oe-init-build-env
    build$ bitbake core-image-apd-minimal

## Building using *AUTOSAR SDK*

In order to use the AUTOSAR SDK you first need to source it:

    $ source ~/autosar_sdk/environment-setup-i586-poky-linux

To build the PackageManager using the AUTOSAR SDK perform the following steps:

    $ cd ~/autosar_sources/sample-applications/ucm_examples
    $ mkdir build
    $ cd build
    $ cmake
    $ make

You may need to import the yocto image first using 'docker import /path/to/poky/build/tmp/deploy/images/qemux86/core-image-apd-devel-qemux86.tar.bz2 ara-apd:test' as is described in the wiki (https://wiki.autosar.org/doku.php?id=ap_dev_guide:guidelines:unit_testing)
