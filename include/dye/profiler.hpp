//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __DYE_PROFILER_HPP__
#define __DYE_PROFILER_HPP__

#include "basic_dye.hpp"

namespace dye {

template<class Base>
class profile_decorator : public Base {

public:
    static profile_decorator * get_instance() { return instance; }

    void begin(const std::string &file, const int line,
               const std::string &function,
               const std::string &section = std::string(),
               const operation_type operation = generic_operation)
    {
        Base::begin(file, line, function, section, operation);
    }

    void end()   { Base::end();   }
    void reset() { Base::reset(); }

    std::string get_call_graph()
    {
        return "get_call_graph not yet implemented";
    }

    std::string get_flat_profile()
    {
        return "get_flat_profile not yet implemented";
    }

    void print_call_graph(std::ostream &output_stream = std::clog)
    {
        output_stream << get_call_graph() << std::endl;
    }

    void print_flat_profile(std::ostream &output_stream = std::clog)
    {
        output_stream << get_flat_profile() << std::endl;
    }

protected:
    profile_decorator() { }

private:
    static profile_decorator * instance;
};


typedef profile_decorator<basic_tracer> profiler;

}

#endif
