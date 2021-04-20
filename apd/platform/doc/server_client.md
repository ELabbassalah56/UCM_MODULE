Server/Client Pipe Based Communication
======================================

In addition to named pipes for low-level input and output, this library offers classes NamedPipeClient and NamedPipeServer to allow for generic, message-based communication between a pair of named pipes in a client and server role. These classes have same limitations as NamedInputPipe/NamedOutputPipe.

Class apd::platform::NamedPipeClient
-----------

See ara::exec::ApplicationClient


Class apd::platform::NamedPipeServer
-----------

See ara::exec::internal::ApplicationStateServer and ara::exec::MachineStateServer.

