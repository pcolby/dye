//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __PROFILE_MACROS_HPP__
#define __PROFILE_MACROS_HPP__

#define PROFILE_MACRO_UUID 4029617a-ebbc-11e3-a02d-080027989a56

#define PROFILE_BEGIN_SCOPE(...) \
    int todo_raii_object##PROFILE_MACRO_UUID=0; \
    todo_raii_object##PROFILE_MACRO_UUID++;   \
    dye::dye_type::get_instance()->begin(__FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__); \
    /// @todo Begin scope.

#endif // __PROFILE_MACROS_HPP__
