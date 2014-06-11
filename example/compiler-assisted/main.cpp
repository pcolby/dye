//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dye/logger.hpp>
#include <dye/profiler.hpp>
#include <dye/macros.hpp>
DYE_DECLARE_TYPE(dye::profile_decorator<dye::logger>);
#include <dye/compiler_assist.hpp>

DYE_DECLARE_INSTANCE();
DYE_DECLARE_ATEXIT_FUNCTION(print_flat_profile);

#include "foo.h"
#include "bar.h"

#include <stdlib.h> // For atexit.

int main(int, char **)
{
    DYE_REGISTER_ATEXIT_FUNCTION(print_flat_profile);
    DYE_BEGIN_SCOPE();

    Foo foo;
    foo.do_something();

    Bar bar;
    bar.do_something();
}
