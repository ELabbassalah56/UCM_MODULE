Safety and security aspects of the implementation {#safety_and_security}
================================

This section covers security and safety issues of the current
implementation.

# Security {#security}

As the package format is not yet fixed in the specification UCM does
not implement or employ signature and authorization checks offered
by AdaptiveCryptoInterface. This is planned for later releases.

There is currently no enforced authorization check of requests to UCM's
service interface. In particular, the transfer id assigned to the client
on TransferStart and used in subsequent calls to other methods of the
service interface shall not be regarded as a security feature.
For future releases, an implementation of a PEP shall adress these
authorization issues.

# Safety {#safety}

The implementation is currently not safe against interruption of package
processing and may not be able to recover from power loss or similar in
all operation states.
In particular, if power loss occurs during processing, activation or
rollback phase, UCM will be in an inconsistent state as no recovery
mechanisms are implemented.
