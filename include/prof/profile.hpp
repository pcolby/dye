//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __PROFILE_PROFILE_HPP__
#define __PROFILE_PROFILE_HPP__

/// @todo Lots

class basic_profiler {

public:
    void begin() const throw() {}
    void end()   const throw() {}
    void reset() const throw() {}

};

template<class Decorant>
class profiler_a : Decorant {

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

#endif // __PROFILE_PROFILE_HPP__
