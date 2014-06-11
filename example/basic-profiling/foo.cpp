//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "foo.h"
#include "dye.h"

Foo::Foo()
{
    DYE_BEGIN_SCOPE();
}

Foo::~Foo()
{
    DYE_BEGIN_SCOPE();
}

void Foo::do_something()
{
    DYE_BEGIN_SCOPE();
}
