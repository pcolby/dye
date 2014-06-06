//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <prof/profile.hpp>
namespace trace {
typedef profiler_a<profiler_b<tracer<basic_profiler> > > profiler;
}
#include <prof/macros.hpp>

template<> trace::profiler * trace::profiler::instance(new trace::profiler);

template<typename Type>
Type factorial_iterative(const Type n)
{
    PROFILE_BEGIN_SCOPE()
    Type result = 1;
    for (Type i = n; i > 1; --i) {
        PROFILE_BEGIN_SCOPE("multiplication")
        result *= i;
    }
    return result;
}

template<typename Type>
Type factorial_recursive(const Type n)
{
    PROFILE_BEGIN_SCOPE()
    return (n == 1) ? 1 : n * factorial_recursive(n-1);
}

class A {
public:
    void B(int) {
        PROFILE_BEGIN_SCOPE();
    }

    void B(double) {
        PROFILE_BEGIN_SCOPE();
    }
};

int main(int, char **)
{
    factorial_iterative<int>(10);
    factorial_recursive<int>(10);

    A a;
    a.B(123);
    a.B(123.0);
    return 0;
}
