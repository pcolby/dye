# Dye

Dye is an extensible, header-only library for tracing C++ code execution via
manual and/or compiler-assisted instrumentation.  Extensibility is provided by
static template-based decoration for minimal runtime overhead, while allowing
versatile extensions, such as trace loggers, instrumented profilers,
and tracers that export to external services such as New Relic.

## Requirements

The basic tracing framework requires little more than a decent C++ compiler.
However, each extension may impose addition dependency if you choose to use
them.

Extension | Requirements
----------|----------------------------
logger    | No additional requirements.
new_relic | New Relic Agent SDK
profiler  | Boost.Thread

## Installation

Installing the library.
```
mkdir build
cd build
cmake <path to dye project>
make install
```

Building the examples.
```
mkdir build
cd build
cmake <path to dye project>
make
```

## To do

* Consistent timestamping.
* Implement dye::profile_decorator.
* Add examples:
  * basic-logging[-disabled]
  * basic-profiling[-disabled]
  * compiler-assisted[-disabled] -> with both logging and profiling.
  * custom-decorator
  * shared-libray
* Add New Relic support.
* Of course, lots of documentation.
