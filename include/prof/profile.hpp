//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __PROFILE_PROFILE_HPP__
#define __PROFILE_PROFILE_HPP__

/// @todo Lots

#include <iostream>

namespace trace {

class basic_profiler {

public:
    void begin(const std::string &, const int, const std::string &,
               const std::string &) const throw() {}
    void end()   const throw() {}
    void reset() const throw() {}

};

template<class Decorant>
class tracer : public Decorant {

public:
    static tracer * get_instance() { return instance; }

    void begin(const std::string &file, const int line,
               const std::string &function, const std::string &section)
    {
        /// @todo  Check for NULL.
        *output_stream
            << file << ':' << line << ':' << function
            << ' ' << section << std::endl;
        Decorant::begin(file, line, function, section);
    }

    /// @todo  Check for NULL.
    void end()   { *output_stream << "end"   << std::endl; Decorant::end();   }
    void reset() { *output_stream << "reset" << std::endl; Decorant::reset(); }

    void set_output_stream(std::ostream * const stream)
    {
        output_stream = stream;
    }

protected:
    std::ostream * output_stream;

    tracer() : output_stream(&std::clog) { }

private:
    static tracer * instance;
};

template<class Decorant>
class profiler : public Decorant {

public:
    static profiler * get_instance() { return instance; }

    void begin(const std::string &file, const int line,
               const std::string &function,
               const std::string &section = std::string())
    {
        Decorant::begin(file, line, function, section);
    }

    void end()   { Decorant::end();   }
    void reset() { Decorant::reset(); }

private:
    static profiler * instance;
};

}

#endif // __PROFILE_PROFILE_HPP__
