Implementation Details {#implementation_details}
=================================================

# Base technology for A/B switching {#nvm}

UCM implements A/B switching using version-tagged software cluster
directories inside the platform's filesystem and a common file which
lists the active software clusters processes in \ref processes_list.

UCM does currently not modify other Functional Clusters, nor system
components like kernel and base libraries.

## Filesystem layout {#filesystem_layout}

UCM installs each software cluster by shortname in a different directory.
Subdirectories are used for different versions so they can be installed
side by side to enable rollback. If Finish() is called the old/removed
version is removed from the filesystem or in case of previous rollback the
updated/installed version is removed.

To illustrate this a simple example is used. Consider a system which
has these software clusters installed:

| Software cluster shortname | Version |
|----------------------------|---------|
| swcl_a             | 1.0.0.3 |
| swcl_b             | 1.0.4.0 |

In the processing phase swcl_a is updated, swcl_b is
removed and swcl_c is added:

| Software cluster shortname | Version |
|----------------------------|---------|
| swcl_a             | 1.0.0.4 |
| swcl_c             | 1.0.0.0 |

The following image shows the structure on the filesystem after activation
has been called.

\image latex apd_ucm.pdf "Directory structure for software clusters" width=12cm

The two versions of swcl_a are installed side by side, swcl_b
is still on the file system and swcl_c has been extracted to the file
system. A new process list was created in PROCESSES_N+1.json and the link is switched
to the new version. Execution management could now be notified to use the updated
processes list to start newly installed processes and/or restart updated ones.

If the system does a rollback to the old state PROCESSES.json is linked back
to PROCESSES_N.json.

Upon finish the obsolete software cluster versions and the obsolete processes list
is removed again.

## Processes list {#processes_list}

The processes list contains all activated platform processes. In the example
use case it would look like this before the processing:

```Javascript
[
    {
        "key": "processes",
        "value": {
            "string[]": [
                "/opt/radar/",
                "/opt/fusion/",
                "/opt/ucm/",
                "/var/lib/apd_ucm/swcl_a/1.0.0.3/application_a1/exec1",
                "/var/lib/apd_ucm/swcl_b/1.0.4.0/application_b1/exec1"
            ]
        },
        "checksum": 870226170
    }
]
```

The new file is created after Activate() is called. It contains updated paths
to the new version of swcl_a, removed the entry for swcl_b's
processes and a new entry for the newly installed swcl_c.

```Javascript
[
    {
        "key": "processes",
        "value": {
            "string[]": [
                "/opt/radar/",
                "/opt/fusion/",
                "/opt/ucm/",
                "/var/lib/apd_ucm/swcl_a/1.0.0.4/application_a1/exec1",
                "/var/lib/apd_ucm/swcl_c/1.0.0.0/application_c1/exec1"
            ]
        },
        "checksum": 8357487
    }
]
```

# Package format {#package_format}

The demonstrator implementation of Update And Configuration Management uses
a vendor-specific package format consisting of a Zip archive with
*defined order of elements* to enable processing in streaming mode.
The following image shows its internal structure and ordering.

\image latex SwPkgtreeview.pdf "UCM Software Package Content" width=11.7cm

Each Software Package addresses one Software Cluster.
For every application inside the software
package it contains subdirectories with the application executables
and their configuration or resource files.

Authentication and integrity protection of data is currently not supported
for the demonstrator implementation due to missing Crypto library.

The contents of the manifest files are explained in the following subsections.

## Software Package manifest {#software_package_manifest}

The software package manifest contains meta information about the software
cluster contained in a software package.
The current implementation uses a JSON representation of the ARXML definition in the
metamodel. Some attributes like compressedSoftwarePackageSize and
uncompressedSoftwarePackageSize are not contained in the manifest as they can
be determined from the size of the archive and its metadata.

The following listing shows the Software Package manifest
(SWPKG_MANIFEST.json) for an installation package.

```Javascript
{
    "actionType": "Install",
    "activationAction": "ApplicationRestart",
    "category": "examplePACKAGE",
    "isDeltaPackage": "false",
    "minUCMSupportedVersion": "1.0.0_0",
    "maxUCMSupportedVersion": "1.0.0_0",
    "packagerID": "1347111236",
    "postVerificationReboot": "false",
    "preActivationReboot": "false",
    "shortName": "SWP_TEST1_INS",
    "uuid": "079af322-1570-11e9-8e05-e79fcc9bd29c"
}
```

The Software Cluster manifest for this package (SWCL_MANIFEST.json)
contains this information.

```Javascript
{
    "category": "exampleSWCL",
    "changes": "Initial release",
    "conflicts": [
        {
            "operator": ">=",
            "shortName": "SWCL_TEST1_VENDOR_B",
            "version": "0.0.0_0"
        }
    ],
    "depends": [
        {
            "operator": ">=",
            "shortName": "SWCL_BASE",
            "version": "1.0.0_0"
        }
    ],
    "diagnosticAddress": "17473",
    "license": "AUTOSAR",
    "shortName": "SWCL_TEST1_VENDOR_A",
    "typeApproval": "None",
    "uuid": "0925f048-1570-11e9-8000-4f62f9ef99ce",
    "vendorID": "1447315780",
    "version": "0.1.0_1547190872"
}
```

Currently no delta packages are implemented, i.e. every package replaces
the existing software cluster during processing. For delta package type packages
the install steps shall be applied on the existing software cluster on
the NVM, i.e. they shall be merged.
