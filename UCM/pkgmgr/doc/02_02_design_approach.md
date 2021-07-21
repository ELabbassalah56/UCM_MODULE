Design approach {#design_approach}
==================================

# Components {#design_components}

\image latex ucm_component_model.pdf "Update And Configuration Management software - high level component diagram" width=12cm

- **Package Manager** software component is the central component of UCM
implementation. It has the following components.
  - **Transfer** implements different means to receive an update
  - **Extraction** implements different extraction methods
  - **Parsing** handles the packages and UCM's platform metadata
  - **Storage** is responsible for writing packages to the Non-Volatile Memory (NVM)
  - **StateMgmt** handles internal state dependent behaviour

The Update And Configuration Management cluster processes software clusters
containing a set of applications. It offers services for installing,
 uninstalling and updating software clusters and their content.

# State management {#state_management}

State management is implemented with specialized classes derived from generic
class PackageManagerState. The default implementations return error
codes. States which support the callable functions override these in their
internal implementation. The following figure shows which methods are
overridden.

\image latex ucm_comp_state.pdf "State management classes" width=14cm

The following table describes the implemented state transitions. The event column lists
the possible external events *Activate*, *Cancel*, *Finish*, *ProcessSwPackage*,
*RevertProcessedSwPackages* and *Rollback* with a prefix '<' to indicate incoming direction.
Internal events are prefixed with '$' resp. '!' for error events.
The columns current state and next state list the state transition. The action column
shows which action is performed in the state. The prefix '>' shows outgoing messages
to the client.

Most client requests are answered after a state transition, e.g. the call to *Activate* is
received in state *Ready* but answered in *Activating*.

Other external events, e.g. *TransferStart* have no influence on the state changing
and are therefore not listed here to enhance clarity.

| Event                                                    | Current state | Next state   | Action                                                               |
|----------------------------------------------------------|---------------|--------------|----------------------------------------------------------------------|
| <Activate                                                | Activated     | Activated    | >ApErr: OperationNotPermitted                                        |
| <Cancel                                                  | Activated     | Activated    | >ApErr: OperationNotPermitted                                        |
| <Finish                                                  | Activated     | Cleaning-Up  | State change                                                         |
| <ProcessSwPackage                                        | Activated     | Activated    | >ApErr: OperationNotPermitted                                        |
| <RevertProcessedSwPackages                               | Activated     | Activated    | >ApErr: OperationNotPermitted                                        |
| <Rollback                                                | Activated     | Rolling-back | State change                                                         |
| $EnterState (from Ready)                                 | Activating    | Activating   | Check dependencies                                                   |
| $Success                                                 | Activating    | Verifying    | >Activate                                                            |
| !ErrorDuringActivation                                   | Activating    | Ready        | >ApErr (Activate): ErrorDuringActivation                             |
| !ErrorNoValidProcessing                                  | Activating    | Ready        | >ApErr (Activate): ErrorNoValidProcessing                            |
| !MissingDependencies                                     | Activating    | Ready        | >ApErr (Activate): MissingDependencies                               |
| <Activate                                                | Activating    | Activating   | >ApErr: OperationNotPermitted                                        |
| <Cancel                                                  | Activating    | Activating   | >ApErr: OperationNotPermitted                                        |
| <Finish                                                  | Activating    | Activating   | >ApErr: OperationNotPermitted                                        |
| <ProcessSwPackage                                        | Activating    | Activating   | >ApErr: OperationNotPermitted                                        |
| <RevertProcessedSwPackages                               | Activating    | Activating   | >ApErr: OperationNotPermitted                                        |
| <Rollback                                                | Activating    | Activating   | >ApErr: OperationNotPermitted                                        |
| $EnterState from Activated (Finish)                      | Cleaning-Up   | Cleaning-Up  | Finalize the activation: (CommitChanges() for each processed Action) |
| $EnterState from Ready (RevertProcessedSwPackages)       | Cleaning-Up   | Cleaning-Up  | Finalize the revert: (RevertChanges() for each processed Action)     |
| $EnterState from Rolled-back (Finish)                    | Cleaning-Up   | Cleaning-Up  | Finalize the rollback: (RevertChanges() for each processed Action)   |
| $Success Finish                                          | Cleaning-Up   | Idle         | >Finish                                                              |
| $Success RevertProcessedSwPackages                       | Cleaning-Up   | Idle         | >RevertProcessedSwPackages                                           |
| !NotAbleToRevertPackages (for RevertProcessedSwPackages) | Cleaning-Up   | Ready        | >ApErr (RevertProcessedSwPackages): NotAbleToRevertPackages          |
| !NothingToRevert (for RevertProcessedSwPackages)         | Cleaning-Up   | Ready        | >ApErr (RevertProcessedSwPackages): NothingToRevert                  |
| <Activate                                                | Cleaning-Up   | Cleaning-Up  | >ApErr: OperationNotPermitted                                        |
| <Cancel                                                  | Cleaning-Up   | Cleaning-Up  | >ApErr: OperationNotPermitted                                        |
| <Finish                                                  | Cleaning-Up   | Cleaning-Up  | >ApErr: OperationNotPermitted                                        |
| <ProcessSwPackage                                        | Cleaning-Up   | Cleaning-Up  | >ApErr: OperationNotPermitted                                        |
| <RevertProcessedSwPackages                               | Cleaning-Up   | Cleaning-Up  | >ApErr: OperationNotPermitted                                        |
| <Rollback                                                | Cleaning-Up   | Cleaning-Up  | >ApErr: OperationNotPermitted                                        |
| <Activate                                                | Idle          | Idle         | >ApErr: OperationNotPermitted                                        |
| <Cancel                                                  | Idle          | Idle         | >ApErr: OperationNotPermitted                                        |
| <Finish                                                  | Idle          | Idle         | >ApErr: OperationNotPermitted                                        |
| <ProcessSwPackage                                        | Idle          | Processing   | State change                                                         |
| <RevertProcessedSwPackages                               | Idle          | Idle         | >ApErr: OperationNotPermitted                                        |
| <Rollback                                                | Idle          | Idle         | >ApErr: OperationNotPermitted                                        |
| $EnterState (from Ready)                                 | Processing    | Processing   | Create cancelable processing thread and start processing             |
| $Success Cancel                                          | Processing    | Processing   | >Cancel                                                              |
| $Success ProcessSwPackage                                | Processing    | Ready        | >ProcessSwPackage                                                    |
| !CancelFailed (for Cancel)                               | Processing    | Processing   | >ApErr (Cancel): CancelFailed                                        |
| !InsufficientMemory                                      | Processing    | Ready/Idle   | >ApErr (ProcessSwPackage): InsufficientMemory                        |
| !InvalidManifest                                         | Processing    | Ready/Idle   | >ApErr (ProcessSwPackage): InvalidManifest                           |
| !InvalidTransferId (for Cancel)                          | Processing    | Processing   | >ApErr (Cancel): InvalidTransferId                                   |
| !InvalidTransferId (for ProcessSwPackage)                | Processing    | Ready/Idle   | >ApErr (ProcessSwPackage): InvalidTransferId                         |
| !ProcessedSwPackageInconsistent                          | Processing    | Ready/Idle   | >ApErr (ProcessSwPackage): ProcessedSwPackageInconsistent            |
| !ProcessSwPackageCancelled                               | Processing    | Ready/Idle   | >ApErr (ProcessSwPackage): ProcessSwPackageCancelled                 |
| <Activate                                                | Processing    | Processing   | >ApErr: OperationNotPermitted                                        |
| <Cancel                                                  | Processing    | Processing   | Set cancel flag                                                      |
| <Finish                                                  | Processing    | Processing   | >ApErr: OperationNotPermitted                                        |
| <ProcessSwPackage                                        | Processing    | Processing   | >ApErr (ProcessSwPackage): ServiceBusy                               |
| <RevertProcessedSwPackages                               | Processing    | Processing   | >ApErr: OperationNotPermitted                                        |
| <Rollback                                                | Processing    | Processing   | >ApErr: OperationNotPermitted                                        |
| <Activate                                                | Ready         | Activating   | State change                                                         |
| <Finish                                                  | Ready         | Ready        | >ApErr: OperationNotPermitted                                        |
| <ProcessSwPackage                                        | Ready         | Processing   | State change                                                         |
| <RevertProcessedSwPackages                               | Ready         | Cleaning-Up  | State change                                                         |
| <Rollback                                                | Ready         | Ready        | >ApErr: OperationNotPermitted                                        |
| <Activate                                                | Rolled-back   | Rolled-back  | >ApErr: OperationNotPermitted                                        |
| <Cancel                                                  | Rolled-back   | Rolled-back  | >ApErr: OperationNotPermitted                                        |
| <Finish                                                  | Rolled-back   | Cleaning-Up  | State change                                                         |
| <ProcessSwPackage                                        | Rolled-back   | Rolled-back  | >ApErr: OperationNotPermitted                                        |
| <RevertProcessedSwPackages                               | Rolled-back   | Rolled-back  | >ApErr: OperationNotPermitted                                        |
| <Rollback                                                | Rolled-back   | Rolled-back  | >ApErr: OperationNotPermitted                                        |
| $EnterState (from Verifying or Activated)                | Rolling-back  | Rolling-back | Do the rollback (previous state of applications restored)            |
| $Success                                                 | Rolling-back  | Rolled-back  | >Rollback                                                            |
| !NotAbleToRollback (from Activated)                      | Rolling-back  | Activated    | >ApErr for Rollback: NotAbleToRollback                               |
| !NotAbleToRollback (from Verifying)                      | Rolling-back  | Verifying    | >ApErr for Rollback: NotAbleToRollback                               |
| !NothingToRollback (from Activated)                      | Rolling-back  | Activated    | >ApErr for Rollback: NothingToRollback                               |
| !NothingToRollback (from Verifying)                      | Rolling-back  | Verifying    | >ApErr for Rollback: NothingToRollback                               |
| <Activate                                                | Rolling-back  | Rolling-back | >ApErr: OperationNotPermitted                                        |
| <Cancel                                                  | Rolling-back  | Rolling-back | >ApErr: OperationNotPermitted                                        |
| <Finish                                                  | Rolling-back  | Rolling-back | >ApErr: OperationNotPermitted                                        |
| <ProcessSwPackage                                        | Rolling-back  | Rolling-back | >ApErr: OperationNotPermitted                                        |
| <RevertProcessedSwPackages                               | Rolling-back  | Rolling-back | >ApErr: OperationNotPermitted                                        |
| <Rollback                                                | Rolling-back  | Rolling-back | >ApErr: OperationNotPermitted                                        |
| $EnterState (from Activating)                            | Verifying     | Verifying    | Start thread to monitor application / platform restart               |
| $Success                                                 | Verifying     | Activated    | State change                                                         |
| !Fail                                                    | Verifying     | Rolling-back | State change                                                         |
| <Activate                                                | Verifying     | Verifying    | >ApErr: OperationNotPermitted                                        |
| <Cancel                                                  | Verifying     | Verifying    | >ApErr: OperationNotPermitted                                        |
| <Finish                                                  | Verifying     | Verifying    | >ApErr: OperationNotPermitted                                        |
| <ProcessSwPackage                                        | Verifying     | Verifying    | >ApErr: OperationNotPermitted                                        |
| <RevertProcessedSwPackages                               | Verifying     | Verifying    | >ApErr: OperationNotPermitted                                        |
| <Rollback                                                | Verifying     | Rolling-back | Cancel monitor thread and do state change                            |

# Data flow {#data_flow}

UCM basically has two data stores used for operation. First there is
a cache used to temporarily store transferred packages. The cache is used for
all clients, i.e. they can transfer packages in parallel as long as storage
and processing resources for the transfer are available.

This cache is then used for actual processing of the packages, i.e. making
modifications to the platform's NVM storage. The NVM storage is also used
to store package processing logs which enable investigation of error cases.
The report process reads from both NVM and also the cache to answer client
requests on available software. It also makes logging data on package
processing available to the client.

UCM's API is intended to support an "A/B" NVM schema to update on inactive
storage and then switch that storage to become the active one. This could be
implemented e.g. using two partitions in a dual bank setup, a filesystem
supporting snapshots and subvolumes like btrfs, a versioning system like
OSTree or a directory hierarchy.

A concrete implementation is to a certain degree hardware dependent as it
has to interact with the bootloader, i.e. to choose active partitions and
configure watchdogs.

The demonstrator uses a custom transaction system based on versioned
directories with an atomically switched index file. It is described in
\ref nvm.

The following figure shows the data flow for UCM.

\image latex ucm_pkgmgr_data_flow.pdf "Data flow of Package Manager" width=14cm

# Control flow {#control_flow}

Due to safety requirements in case of interrupted updates UCM package
manager's interface towards Diagnostic Service Applications (DSA)
is very state dependent.

Currently only version requests and logging can be called independent of
the state. Therefore the internal implemenation also follows a specific path.
The following picture shows a control flow diagram of the Package Manager.

\image latex ucm_pkgmgr_ctrl_flow.pdf "Control flow of Package Manager" width=14cm
