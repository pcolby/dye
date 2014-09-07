//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __DYE_BASIC_DYE_HPP__
#define __DYE_BASIC_DYE_HPP__
#ifdef DYE_ENABLED

#include <string>

namespace dye {

enum operation_type {
    generic_operation,
    datastore_create, ///< eg INSERT, SendMessage
    datastore_read,   ///< eg SELECT, ReceiveMessage
    datastore_update, ///< eg UPDATE, Reject/release
    datastore_delete, ///< eg DELETE, Acknowledge / delete
};

class basic_dye {

public:
    void begin(const std::string &, const int, const std::string &,
               const std::string &, const operation_type) const throw()
        __attribute__((no_instrument_function)) {}

    void end()   const throw() __attribute__((no_instrument_function)) {}

    void reset() const throw() __attribute__((no_instrument_function)) {}

protected:
    basic_dye() throw() {}

};

}

#endif

#endif
