//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "foo.h"
#include "bar.h"
#include "dye.h"

DYE_DECLARE_INSTANCE();
DYE_DECLARE_ATEXIT_FUNCTION(print_flat_profile);

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
