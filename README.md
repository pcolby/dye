# Trace++ or maybe t++ or tpp?

Trace++ is an extensible, header-only library for tracing C++ code execution
via manual and/or compiler-assisted instrumentation.  Extensibility is
provided by static template-based decoration for minimal runtime overhaed,
while allowing versatile extensions, such as trace loggers, instrumented
profilers, and tracers that export to external services such as New Relic.

Some random thoughts:

classes:
* prof::basic_profiler
* prof::trace_profiler

Then again, trace_profiler is not even a profiler.

Maybe this should be a "tracer" project?


trace::null
trace::log
trace::profile
trace::new_relic

trace::log<trace::profile<trace::null>>

trace::log_decorator
trace::profile_decorator
trace::logger


typdedef trace::profiler trace_type;
typdedef trace::new_relic trace_type;
typdedef trace::new_relic_decorator<trace::profile_decorator<trace::logger> > trace_type;

