//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __DYE_SCOPE_GUARD_HPP__
#define __DYE_SCOPE_GUARD_HPP__
#ifdef DYE_ENABLED

namespace dye {

/// @todo  Extend this to not need to know in advance the instance type.

template <class DyeType, typename CountType>
class scope_guard {

public:

    scope_guard(const CountType start_count = 0) : count(start_count)
    {
    }

    ~scope_guard()
    {
        DyeType * const instance = DyeType::get_instance();
        for (;count > 0; --count) {
            instance->end();
        }
    }

    /**
     * @brief Pre-increment the counter.
     *
     * @return the new count.
     */
    CountType operator++()
    {
        return ++count;
    }

    /**
     * @brief Post-increment the counter.
     *
     * @return the new count.
     */
    CountType operator++(CountType)
    {
        return count++;
    }

    /**
     * @brief Pre-decrement the counter.
     *
     * @return the new count.
     */
    CountType operator--()
    {
        return --count;
    }

    /**
     * @brief Post-decrement the counter.
     *
     * @return the new count.
     */
    CountType operator--(CountType)
    {
        return count--;
    }

private:
    CountType count;

};

}

#endif
#endif
