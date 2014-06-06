//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __DYE_BASIC_DYE_HPP__
#define __DYE_BASIC_DYE_HPP__

#include <string>

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

}

#endif
