# Dye

Dye is an extensible, header-only library for tracing C++ code execution via
manual and/or compiler-assisted instrumentation.  Extensibility is provided by
static template-based decoration for minimal runtime overhaed, while allowing
versatile extensions, such as trace loggers, instrumented profilers,
and tracers that export to external services such as New Relic.
