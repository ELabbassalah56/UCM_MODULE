Basic low level pipe communication
==================================

This library offers helper classes for work with named pipes. The classes are specialized for sending/receiving data in form of C++ structures. There are serious limitations: 
* the structures fields should contain all data. It means the structures cannot have fields-pointers or fields-references.
* sizeof(<structure type>) should be less than PIPE_BUF.

Class apd::platform::NamedInputPipe
-----------------------------------
Class NamedInputPipe is used to read C++ structures from a particular pipe.

```
#include "apd/platform/named_input_pipe.h"
..
struct A {
    int8_t field_;
    int8_t array[3];
};
...
apd::platform::NamedInputPipe inputPipe("/opt/pipe-name",...);
A a;
if (inputPipe.read(&a)) {
    a.field_; // use a here
}
```


Class apd::platform::NamedOutputPipe
------------------------------------
Class NamedOutputPipe is used to write C++ structures to a particular pipe.
```
#include "apd/platform/named_output_pipe.h"
..
struct A {
    int8_t field_;
    int8_t array[3];
};
...
apd::platform::NamedOutpuPipe outputPipe("/opt/pipe-name",...);
A a = {1, {1, 2, 3}};
outputPipe.write(&a));
```
