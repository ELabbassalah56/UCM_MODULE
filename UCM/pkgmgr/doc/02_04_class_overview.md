Class Overview {#class_overview}
================================

# Package Manager {#package_manager}

Package Manager is the main deliverable provided by Update And Configuration
 Management functional cluster. It implements most of the features described
  in the Update And Configuration Management specification.

\image latex ucm_class_diagram.pdf "Package Manager - class diagram" width=16cm

The Package Manager instantiates a PackageManagementApplication object that handles
the operation of the service interface and signals (e.g. SIGTERM).

The main service provided by UCM's Package Manager is ProcessSwPackage.

The function extracts a software package to a temporary location. It then
parses the software package manifest to check the operation type requested.

# Receive component {#receive}

In order to provide multiple means of receiving update packages Package
Manager uses a ReceivePackageInterface which can be implemented by different
providers. Currently only a streaming provider is implemented to receive
packages via diagnostics.

The following figure shows the component overview.

\image latex ucm_comp_receive.pdf "Receive component" width=14cm

## Streamable software package

The streamable software package covers the traditional transmission of an
update package to an ECU via UDS diagnostics using the UCM service
methods TransferStart, TransferData and TransferExit.

The service shall support concurrent uploads by multiple clients, therefore
the design is separated into multiple classes and interfaces.
The incoming service interface request is received by the package manager
and forwarded to an
individual object instance of StreamableSoftwarePackage retrieved from
SynchronizedStorage which maps package objects by transfer ids.

Upon receiving a TransferStart, the SynchronizedStorage generates a new
TransferId, instantiates a new StreamableSoftwarePackage object and maps both in a
data structure. It then calls TransferStart on the new object with parameter
size forwarded from the original request. The TransferId is hidden from the
instance object and only used inside the SynchronizedStorage to map subsequent
incoming calls to TransferData and TransferExit to the related instance object.
The size value is stored inside the instance object and it opens a file handle
for storing the data. The transfer state is set to Transferring as shown in the
following picture and the function returns. The PackageManager's
TransferStart then creates the client return value by combining the generated
TransferId with the result of the object instance call TransferStartSuccessType
to a TransferStartReturnType.

\image latex ucm_receive_transfer_state.pdf "Transition of internal transfer states" width=14cm

If TransferData or TransferExit is now called at PackageManager, it
looks up the instance object associated with the provided TransferId,
calls the objects TransferData or TransferExit methods and returns the
objects result to the client. If no instance object is found, an error is
directly returned to the client, therefore this transition is not included
in the instance state machine.

A TransferInstance object in state Transferring has to keep track of and
compare size of the incoming data in TransferData to the expected size set on
TransferStart. It has to detect overflow of data as well as a TransferExit
without enough data transmitted before. If the size matches it will move to
Transferred state, otherwise to Error state.

# Extraction component {#extraction}

As the package archive format is not yet fixed or may be left as
implementation specific the extraction is abstracted to an interface
that can be implemented by different providers. The demonstrator currently
implements the LibPocoZipExtractor.

The following figure shows the component overview.

\image latex ucm_comp_extract.pdf "Extract component" width=10cm

# Parsing component {#parsing}

The parsing component has interfaces to the Package Manager which
enable the latter one to get information on the installed components.
The SoftwareClusterListBuilder indexes the content of the platform and
makes it available via the version report interface. For implementing
the A/B functionality two instances are run, one on the active and the
other on the inactive partition.

The following figure shows the component overview.

\image latex ucm_comp_parsing.pdf "Parsing component" width=14cm

# Storage components {#storage}

As a software package can contain an \ref install_action, a \ref remove_action,
or an \ref update_action request,
different actions are created by the action generator
depending on the input SoftwarePackage passed to the PackageManager's
ProcessSwPackage() method.

The ReversibleAction class defines three virtual methods to be implemented by subclasses:

- An Execute() method which is called when the ProcessSwPackage() is called on that package.
- A RevertChanges() method which reverts all changes done in the Execute method.
- A CommitChanges() method which finalizes the changes done in the Execute method.

The methods RevertChanges() and CommitChanges() are called when entering
the CleaningUp state. If entering the state via a RevertProcessedSwPackages()
call from the client RevertChanges() is called. If entering the state due
to a Finish() call the executed method depends on whether a rollback was
done.

|                             | RevertChanges() | CommitChanges() |
|-----------------------------|-----------------|-----------------|
| Finish()                    |                 |        X        |
| Finish() after Rollback()   |        X        |                 |
| RevertProcessedSwPackages() |        X        |                 |

The storage component implements the class SWCLManager which indexes the
platforms NVM for software clusters directories and parses their manifest
files. It provides
function AddSWCLChangeInfo() which adds the changes done by a specific software
package. This information is accessed by GetSwClusterChangeInfo(). After
Finish() the change information is reset by calling ResetSWCLChangeInfo() and
the SWCLManager reindexes the present SWCLs to answer client requests
to GetSwClusterInfo().

The following figure shows the component overview.

\image latex ucm_comp_storage.pdf "Storage component" width=14cm

## InstallAction {#install_action}

In case of an InstallAction, the SoftwarePackage object
created by the parsing component is
processed by the action. It contains references to the
SoftwareCluster's artifacts that will be written to the filesystem.
For the Execute method it moves the extracted software cluster
directory to the final location into a subdirectory named by
its version number, see \ref file_system_layout for details.
A reference is added in the SWCLManager by calling AddSWCL with
the state set to kAdded.
For the RevertChanges method it removes the
newly created directory and the reference in the SWCLManager.
On CommitChanges no action
is taken as the software cluster is kept and its state changes
to kPresent in the SWCLManager.

## RemoveAction {#remove_action}

In case of a RemoveAction, the Execute method sets the software cluster
as kRemoved in the SWCLManager.
The RevertChanges sets its state back to kPresent.
On CommitChanges the software cluster
directory is removed. The software cluster is removed from the
SWCLManager.

## UpdateAction {#update_action}

An update extracts the software clusters new version to the file
system and sets the associated Software Cluster object in the
SWCLManager to state kUpdated on Execute call.
On RevertChanges the new version is removed again
from the filesystem.
On CommitChanges the old version is removed from
the filesystem.

The following table lists the filesystem actions and the state to set
the software cluster to for each method.

|                       | InstallAction | RemoveAction | UpdateAction            |
|-----------------------|---------------|--------------|-------------------------|
| Execute               | move / kAdded | - / kRemoved | move (new) / kUpdated   |
| RevertChanges         | delete / -    | - / kPresent | delete (new) / kPresent |
| CommitChanges         |  - / kPresent | delete / -   | delete (old) / kPresent |

Actions are owned by the FilesystemSWCLManager and implicitly form a transaction
supported by the state machine flow.

## Activate and Rollback {#activation_rollback}

After processing is finished activation can be performed. In this step
the package manager performs dependency checks of the
installed software for all software clusters with state kAdded, kUpdated or kPresent.
If the check is successful it creates a new processes list including the
installed and updated processes whose software cluster are not in state kRemoved
as well as the platform-level applications. After the file is generated it is
switched with the current processes list which is still kept for a
possible rollback.

In case of rollback the backup process list becomes active again
and is send to EM.

## Finish {#finish}

If Finish() has been called after Rollback() it removes everything that
was added during the prior processing, i.e. updated and newly installed
software clusters by calling the RevertChanges function for each ReversibleAction
and deletes the newly created processes list.

If Finish() has been called after Activate() without a rollback it
removes uninstalled software clusters and old versions of updated
software clusters by calling CommitChanges for each ReversibleAction.
The old version of the processes list is also deleted.
