//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __DYE_MACROS_HPP__
#define __DYE_MACROS_HPP__
#ifdef DYE_ENABLED

#include "scope_guard.hpp"

/**
 * @brief UUID to protect against variable name clashes.
 *
 * Some dye macros (such as DYE_BEGIN_SCOPE) declare local variables for things
 * such as automatic section ending on out-of-scope (that is, the RAII pattern).
 * Such variable names are suffixed with this UUID to ensure to that they never
 * clash with your own application variables.
 *
 * There is no need for you to use the DYE_MACRO_UUID macro in your own code,
 * and if doing so you are probably doing something wrong.
 *
 * Note, there's nothing special about this UUID - it was generated via the
 * OSSP uuid command-line tool.  It may be replaced at any time.
 */
#define DYE_MACRO_UUID 4029617a-ebbc-11e3-a02d-080027989a56

#define DYE_BEGIN_SCOPE(...) \
    dye::scope_guard<dye::dye_type,int> dye_scope_guard_##DYE_MACRO_UUID=0; \
    dye::dye_type::get_instance()->begin(__FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__); \
    ++dye_scope_guard_##DYE_MACRO_UUID;

#define DYE_BEGIN_SECTION(...) \
    dye::dye_type::get_instance()->begin(__FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__); \
    ++dye_scope_guard_##DYE_MACRO_UUID;

#define DYE_END_SECTION() \
    dye::dye_type::get_instance()->end(); \
    --dye_scope_guard_##DYE_MACRO_UUID;

#define DYE_DECLARE_TYPE(type) \
    namespace dye { typedef type dye_type; }

#define DYE_DECLARE_INSTANCE() \
    namespace dye { template<> dye_type * dye_type::instance(new dye::dye_type); }

#define DYE_DECLARE_ATEXIT_FUNCTION(method, ...) \
    void dye_##method##_atexit_##DYE_MACRO_UUID() { \
        dye::dye_type::get_instance()->method(##__VA_ARGS__); \
    }

#define DYE_REGISTER_ATEXIT_FUNCTION(method) \
    atexit(dye_##method##_atexit_##DYE_MACRO_UUID)

#else

#define DYE_BEGIN_SCOPE(...)
#define DYE_BEGIN_SECTION(...)
#define DYE_END_SECTION()
#define DYE_DECLARE_TYPE(type)
#define DYE_DECLARE_INSTANCE()
#define DYE_DECLARE_ATEXIT_FUNCTION(method, ...)
#define DYE_REGISTER_ATEXIT_FUNCTION(method)

#endif
#endif
