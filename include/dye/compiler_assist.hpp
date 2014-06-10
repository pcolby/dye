//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __DYE_COMPILER_ASSIST_HPP__
#define __DYE_COMPILER_ASSIST_HPP__

#include "macros.hpp"

#include <cxxabi.h>
#include <dlfcn.h>

#include <stdio.h> // remove?
#include <stdlib.h>

#include <sstream>

namespace dye {
namespace internal {

static size_t cyg_profile_count = 0;

}}

extern "C" {

void __cyg_profile_func_enter(void *this_fn, void *)
{
    dye::dye_type * const dye_instance = dye::dye_type::get_instance();
    if (dye_instance == NULL) {
        return;
    }
    dye::internal::cyg_profile_count++;

    Dl_info symbol_info;
    if ((!dladdr(this_fn, &symbol_info)) || (symbol_info.dli_sname == NULL)) {
        std::ostringstream stream;
        stream << std::hex << this_fn;
        dye_instance->begin(std::string(), 0, stream.str());
        return;
    }

    int demangle_status;
    char * const demangled_name =
        abi::__cxa_demangle(symbol_info.dli_sname, NULL, 0, &demangle_status);
    if (demangle_status == 0) {
        dye_instance->begin(symbol_info.dli_fname, 0, demangled_name);
        free(demangled_name);
        return;
    }

    dye_instance->begin(symbol_info.dli_fname, 0, symbol_info.dli_sname);
}

void __cyg_profile_func_exit(void *, void *)
{
    if (dye::internal::cyg_profile_count > 0) {
        dye::internal::cyg_profile_count--;
        dye::dye_type::get_instance()->end();
    }
}

}

#endif
