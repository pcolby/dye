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

    static log_decorator * get_instance() __attribute__((no_instrument_function));


    void begin(const std::string &file, const int line,
               const std::string &function,
               const std::string &section = std::string(),
               const operation_type operation = generic_operation)
    {
        if (output_stream) {
            *output_stream
                << file << ':' << line << ':';
            for (size_t depth = current_depth; depth > 0; --depth) {
                *output_stream << indent_string;
            }
            *output_stream
                << function
                << ' ' << section << std::endl;
        }
        current_depth++;
        Base::begin(file, line, function, section, operation);
    }

    void end()
    {
        if (current_depth > 0) {
            current_depth--;
        }
        Base::end();
    }

    void reset()
    {
        current_depth = 0;
        Base::reset();
    }

    std::string set_indent_string(const std::string &new_indent)
    {
        const std::string previous_indent = indent_string;
        indent_string = new_indent;
        return previous_indent;
    }

    std::ostream * set_output_stream(std::ostream * const stream)
    {
        std::ostream * const previous_stream = output_stream;
        output_stream = stream;
        return previous_stream;
    }

protected:
    size_t current_depth;
    std::string indent_string;
    std::ostream * output_stream;

    log_decorator() :
        current_depth(0),
        indent_string(" "),
        output_stream(&std::clog)
    {
    }

private:
    static log_decorator * instance;

};

template<class Base>
log_decorator<Base> * log_decorator<Base>::get_instance()
{
    return instance;
}


typedef log_decorator<basic_tracer> logger;

}

#endif
