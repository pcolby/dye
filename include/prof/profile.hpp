//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __PROFILE_PROFILE_HPP__
#define __PROFILE_PROFILE_HPP__

/// @todo Lots

class basic_profiler {
    void begin();
    void end();
    void reset();
};

template<class Decorant>
class profiler_a {

public:
    static profiler_a * get_instance() { return instance; }
    void begin() { Decorant::begin(); }
    void end()   { Decorant::end();   }
    void reset() { Decorant::reset(); }

private:
    static profiler_a * instance;
};

template<class Decorant>
class profiler_b : Decorant {

public:
    static profiler_b * get_instance() { return instance; }
    void begin() { Decorant::begin(); }
    void end()   { Decorant::end();   }
    void reset() { Decorant::reset(); }

private:
    static profiler_b * instance;
};

// This typedef would be done by the application, prior to including this header.
typedef profiler_a<profiler_b<basic_profiler> > profiler;

// This typedef would be done by the application.
template<> profiler * profiler::instance(new profiler);

#define PROFILE_MACRO_UUID 4029617a-ebbc-11e3-a02d-080027989a56

#define PROFILE_BEGIN_SCOPE(...) \
    int todo_raii_object##PROFILE_MACRO_UUID=0; \
    todo_raii_object##PROFILE_MACRO_UUID++;   \
    profiler::get_instance(); \
    /// @todo Begin scope.

#endif // __PROFILE_PROFILE_HPP__
