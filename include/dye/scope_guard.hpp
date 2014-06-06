//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __DYE_SCOPE_GUARD_HPP__
#define __DYE_SCOPE_GUARD_HPP__

namespace dye {

template <typename Type>
class basic_scope_guard {

public:

    basic_scope_guard(const Type start_count = 0) : count(start_count)
    {
    }

    ~basic_scope_guard()
    {
        dye_type * const instance = dye_type::get_instance();
        for (;count > 0; --count) {
            instance->end();
        }
    }

    /**
     * @brief Pre-increment the counter.
     *
     * @return the new count.
     */
    Type operator++()     { return ++count; }

    /**
     * @brief Post-increment the counter.
     *
     * @return the new count.
     */
    Type operator++(Type) { return count++; }

    /**
     * @brief Pre-decrement the counter.
     *
     * @return the new count.
     */
    Type operator--()     { return --count; }

    /**
     * @brief Post-decrement the counter.
     *
     * @return the new count.
     */
    Type operator--(Type) { return count--; }

private:
    Type count;

};

typedef basic_scope_guard<int> scope_guard;

}

#endif
