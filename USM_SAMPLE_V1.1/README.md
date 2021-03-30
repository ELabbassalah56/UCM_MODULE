## TOC
1. Folder structure
2. Implemented features
3. Known limitations


## 1. Folder structure

The folder structure is:

    ucm_examples
    ├── pkgmgr_sample
    ├── swcl_sample


* Folder `pkgmgr_sample` contains the implementation of a sample application for testing the interaction with the PackageManager component
* Folder `swcl_sample` contains the implementation of an application for previewing a sample of the SWCL (Software Clusters).

## 2. Implemented features

The sample application `pkgmgr_sample`
* finds the PackageManagement service that is offered by the PackageManager component
* transfers three different software packages (one for install, update and uninstall)
* calls ProcessSwPackage method of the PackageManagement service for each software package
* activates the changes by calling Activate method of the PackageManagement service

The sample application `swcl_sample`
* logs log messages as an indication that it has been executed.
* creates the SWCL directory structure for dynamic updates in the target image.
* installs a SWCL manifest sample in the target image.


## 3. Known limitations

The current sample app (`pkgmgr_sample`) uses a stub for the PackageManagement service, since the service does not yet exist.
