//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dye/logger.hpp>
#include <dye/profiler.hpp>
namespace dye { typedef profile_decorator<logger> dye_type; }
#include <dye/macros.hpp>

#include <stdlib.h>

template<> dye::dye_type * dye::dye_type::instance(new dye::dye_type);
DYE_DECLARE_ATEXIT_FUNCTION(print_flat_profile);

template<typename Type>
Type factorial_iterative(const Type n)
{
    DYE_BEGIN_SCOPE()
    Type result = 1;
    for (Type i = n; i > 1; --i) {
        DYE_BEGIN_SCOPE("multiplication")
        result *= i;
    }
    return result;
}

template<typename Type>
Type factorial_recursive(const Type n)
{
    DYE_BEGIN_SCOPE()
    return (n == 1) ? 1 : n * factorial_recursive(n-1);
}

class A {
public:
    void B(int) {
        DYE_BEGIN_SCOPE();
    }

    void B(double) {
        DYE_BEGIN_SCOPE();
    }

    void C() {
        DYE_BEGIN_SCOPE();

        DYE_BEGIN_SECTION("first loop");
        for (size_t i = 0; i < 10; ++i) {

        }
        DYE_END_SECTION()

        DYE_BEGIN_SECTION("second loop", dye::generic_operation);
        for (size_t i = 0; i < 10; ++i) {

        }
        DYE_END_SECTION()
    }
};

int main(int, char **)
{
    DYE_REGISTER_ATEXIT_FUNCTION(print_flat_profile);
    dye::dye_type::get_instance()->set_output_stream(&std::cerr);

    factorial_iterative<int>(10);
    factorial_recursive<int>(10);

    dye::dye_type::get_instance()->set_output_stream(&std::cout);

    A a;
    a.B(123);
    a.B(123.0);
    a.C();
    a.C();
    return 0;
}
