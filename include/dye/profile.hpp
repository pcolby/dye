//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __PROFILE_PROFILE_HPP__
#define __PROFILE_PROFILE_HPP__

/// @todo Lots

#include <iostream>

namespace dye {

class basic_tracer {

public:
    void begin(const std::string &, const int, const std::string &,
               const std::string &) const throw() {}
    void end()   const throw() {}
    void reset() const throw() {}

protected:
    basic_tracer() throw() {}
};

template<class Base>
class log_decorator : public Base {

public:
    static log_decorator * get_instance() { return instance; }

    void begin(const std::string &file, const int line,
               const std::string &function, const std::string &section)
    {
        /// @todo  Check for NULL.
        *output_stream
            << file << ':' << line << ':' << function
            << ' ' << section << std::endl;
        Base::begin(file, line, function, section);
    }

    /// @todo  Check for NULL.
    void end()   { *output_stream << "end"   << std::endl; Base::end();   }
    void reset() { *output_stream << "reset" << std::endl; Base::reset(); }

    void set_output_stream(std::ostream * const stream)
    {
        output_stream = stream;
    }

protected:
    std::ostream * output_stream;

    log_decorator() : output_stream(&std::clog) { }

private:
    static log_decorator * instance;
};

typedef log_decorator<basic_tracer> logger;

template<class Base>
class profiler : public Base {

public:
    static profiler * get_instance() { return instance; }

    void begin(const std::string &file, const int line,
               const std::string &function,
               const std::string &section = std::string())
    {
        Base::begin(file, line, function, section);
    }

    void end()   { Base::end();   }
    void reset() { Base::reset(); }

protected:
    profiler() { }

private:
    static profiler * instance;
};

}

#endif // __PROFILE_PROFILE_HPP__
