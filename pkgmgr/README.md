## TOC
1. Folder structure
2. Implemented features
3. Known limitations

## 1. Folder structure

The folder structure is:

	pkgmgr
    ├── doc
    │   └── images
    ├── files
    │   ├── etc
    │   └── manifests
    └── src
        ├── extraction
        ├── interruptible_thread
        ├── package_manager
        ├── package_manager_impl
        ├── package_manager_state
        ├── parsing
        ├── receive
        ├── storage
        ├── test
        ├── transaction
        └── transfer

* Folder `doc` contains the Doxyfile for generating the source code documentation as an HTML page. An additional cmake target `documentation` is created. This is also the input for building the [UCM FCDesign Document](https://svn.autosar.org/repos/work/26_Standards/30_AP_R1/01_Sources/EXP_FCDesignUpdateAndConfigManagement_894/AUTOSAR_EXP_FCDesignUpdateAndConfigManagement.pdf)
* Folder `files` contains any non-source files. Currently only the etc/MANIFEST.json and manifests/system_manifest.arxml are part of this directory.
* Folder `src` contains the source files and private header files.
* Folder `src/test` contains the source files for tests. Currently, only unit tests are implemented.
* Other folders are used to divide the implementation into topic specific modules. Please refer to [UCM FCDesign Document](https://svn.autosar.org/repos/work/26_Standards/30_AP_R1/01_Sources/EXP_FCDesignUpdateAndConfigManagement_894/AUTOSAR_EXP_FCDesignUpdateAndConfigManagement.pdf) for more information.

## 2. Implemented features

See the root README.md file of this FC.

## 3. Known limitations

See the root README.md file of this FC.

## 4. Last Update Section V1.0

develop ucm processing sw pkg and some state incomplete 
   
   * ProcessingSwPackage State Created      following AUTOSAR_EXP_FCUpdate&configrationManagement
    
     - </br>![Status Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)
    
     - </br>![STATE MACHINE](https://github.com/ELabbassalah56/UCM_MOdule/blob/main/pkgmgr/doc/images/StaticAnalysis.png?raw=true)
  * UCM Static Analysis using CPP_Check
    - </br>![Status Status](https://github.com/ELabbassalah56/UCM_MOdule/blob/main/pkgmgr/doc/images/swclState.png?raw=true)
