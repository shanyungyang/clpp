//          Copyright Shan-Yung Yang 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLPP_COMMON_HPP
#define CLPP_COMMON_HPP

#if defined(__APPLE__) || defined(__MACOSX) // Mac OS
#include <OpenCL/cl.h>
#else // Windows or Un*x
#include <CL/cl.h>
#endif

#endif // CLPP_COMMON_HPP
