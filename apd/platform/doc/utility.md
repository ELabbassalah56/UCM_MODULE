Generic Utilities
=================

This contains generic utility functions for working with time, strings and paths, namely:

Time
----

  - Sleep (argument in milliseconds)
  - Current system timestamp (in microseconds)

You should use std::chrono and std::thread instead of these functions.

String
------

  - Trim (configurable) whitespace from a string
  - Prepend and append spaces to a vector of strings

Path
----

  - Get the current working directory

