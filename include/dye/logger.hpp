//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __DYE_LOGGER_HPP__
#define __DYE_LOGGER_HPP__

#include "basic_dye.hpp"

#include <iostream>

namespace dye {

template<class Base>
class log_decorator : public Base {

public:
    static log_decorator * get_instance() { return instance; }

    void begin(const std::string &file, const int line,
               const std::string &function, const std::string &section)
    {
        if (output_stream) {
            *output_stream
                << file << ':' << line << ':' << function
                << ' ' << section << std::endl;
        }
        Base::begin(file, line, function, section);
    }

    /// @todo  Check for NULL.
    void end()
    {
        if (output_stream) {
            *output_stream << "end"   << std::endl; Base::end();
        }
        Base::end();
    }

    void reset()
    {
        if (output_stream) {
            *output_stream << "reset" << std::endl; Base::reset();
        }
        Base::reset();
    }

    std::ostream * set_output_stream(std::ostream * const stream)
    {
        std::ostream * const previous_stream = output_stream;
        output_stream = stream;
        return previous_stream;
    }

protected:
    std::ostream * output_stream;

    log_decorator() : output_stream(&std::clog)
    {
    }

private:
    static log_decorator * instance;

};

typedef log_decorator<basic_tracer> logger;

}

#endif
