#ifndef CLPP_COMMON_HPP
#define CLPP_COMMON_HPP

#if defined(__APPLE__) || defined(__MACOSX) // Mac OS
#include <OpenCL/cl.h>
#else // Windows or Un*x
#include <CL/cl.h>
#endif

#endif // CLPP_COMMON_HPP
