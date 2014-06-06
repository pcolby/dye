# Dye

Dye is an extensible, header-only library for tracing C++ code execution via
manual and/or compiler-assisted instrumentation.  Extensibility is provided by
static template-based decoration for minimal runtime overhaed, while allowing
versatile extensions, such as trace loggers, instrumented profilers,
and tracers that export to external services such as New Relic.

```
mkdir build
cd build
cmake <path to dye project>
make
```

## To do

* Implement RAII auto-ender.
* Implement dye::profile_decorator.
* Add examples:
** basic-logging[-disabled]
** basic-profiling[-disabled]
** compiler-assisted[-disabled] -> with both logging and profiling.
** custom-decorator
** shared-libray
* Add New Relic support.
* Of course, lots of documentation.
