# Dye

Dye is an extensible, header-only library for tracing C++ code execution via
manual and/or compiler-assisted instrumentation.  Extensibility is provided by
static template-based decoration for minimal runtime overhead, while allowing
versatile extensions, such as trace loggers, instrumented profilers,
and tracers that export to external services such as New Relic.

## Requirements

The basic tracing framework requires little more than a decent C++ compiler.
However, each extension may impose addition dependencies if you choose to use
them.

Extension | Requirements
----------|----------------------------
logger    | No additional requirements.
new_relic | New Relic Agent SDK
profiler  | Boost C++ Libraries (Data Time, System, Thread)
Boost.Thread and Boost.

Also, the optional compiler-assisted instrumentation depends on compiler-
specific extensions, such as gcc's `-finstrument-functions` option (also
supported by clang).

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

* Implement dye::profile_decorator.
* Add examples:
  * custom-decorator
  * shared-libray
* Add New Relic support.
* Of course, lots of documentation.
* Rename to Vermeer?
