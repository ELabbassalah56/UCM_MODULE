Interfaces to other functional clusters {#interfaces_to_fcs}
================================

UCM has multiple interfaces to other functional clusters on the Adaptive
Platform as depicted in the figure below. The following subsections
describe the implemented as well as the planned interfaces.

\image latex ucm_interfaces_fcs.pdf "Interfaces to other functional clusters" width=12cm

# State Management {#if_state_management}

For full operation, UCM has to be able to set machine modes, start,
stop or restart Adaptive Applications or Functional Clusters. This is
done using interfaces towards State Management. As some of the
needed interfaces do not yet exists, calls to State Management
are currently stubbed.

# Persistency {#if_persistency}

UCM currently has no implemented interface towards Persistency.
Later releases may contain functionality to merge existing persistency
data of an Adaptive Application with persistency data supplied with a
Software Package.

# Diagnostics {#if_diag}

In order to support Software Clusters having an own diagnostics target
address UCM needs to have interfaces to functional cluster Diagnostics.
These interfaces shall be used to inform Diagnostics about newly
installed or removed Software Clusters so that Diagnostics can properly
handle requests targeted to them.

As UCM currently only implements Software Clusters without a dedicated
diagnostics target address these interfaces are neither
defined nor implemented.

# Identity and Access Management {#if_identity_and_access_management}

UCM currently has no implemented interface towards Identity and Access
Management. An implementation of a Policy Enforcement Point (PEP) is planned
for later releases.
