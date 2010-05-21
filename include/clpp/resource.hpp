//          Copyright Shan-Yung Yang 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLPP_RESOURCE_HPP
#define CLPP_RESOURCE_HPP

#include "common.hpp"
#include "error.hpp"

namespace clpp {

template <typename Handle> struct ResourcePolicy;

template <> struct ResourcePolicy<cl_context> {
    static cl_context null() throw() { return 0; }
    static cl_int retain(cl_context h) { return clRetainContext(h); }
    static cl_int release(cl_context h) throw() { return clReleaseContext(h); }
}; // struct ResourcePolicy<cl_context>

template <> struct ResourcePolicy<cl_command_queue> {
    static cl_command_queue null() throw() { return 0; }
    static cl_int retain(cl_command_queue h) { return clRetainCommandQueue(h); }
    static cl_int release(cl_command_queue h) throw() { return clReleaseCommandQueue(h); }
}; // struct ResourcePolicy<cl_command_queue>

template <> struct ResourcePolicy<cl_mem> {
    static cl_mem null() throw() { return 0; }
    static cl_int retain(cl_mem h) { return clRetainMemObject(h); }
    static cl_int release(cl_mem h) throw() { return clReleaseMemObject(h); }
}; // struct ResourcePolicy<cl_mem>

template <> struct ResourcePolicy<cl_program> {
    static cl_program null() throw() { return 0; }
    static cl_int retain(cl_program h) { return clRetainProgram(h); }
    static cl_int release(cl_program h) throw() { return clReleaseProgram(h); }
}; // struct ResourcePolicy<cl_program>

template <> struct ResourcePolicy<cl_kernel> {
    static cl_kernel null() throw() { return 0; }
    static cl_int retain(cl_kernel h) { return clRetainKernel(h); }
    static cl_int release(cl_kernel h) throw() { return clReleaseKernel(h); }
}; // struct ResourcePolicy<cl_kernel>

template <> struct ResourcePolicy<cl_event> {
    static cl_event null() throw() { return 0; }
    static cl_int retain(cl_event h) { return clRetainEvent(h); }
    static cl_int release(cl_event h) throw() { return clReleaseEvent(h); }
}; // struct ResourcePolicy<cl_event>

/// A general resource wrapper.
template <typename Handle> class Resource {
    public:
        typedef ResourcePolicy<Handle> Policy;

        Resource() : my_handle(Policy::null()) {}

        explicit Resource(Handle h) : my_handle(h) {}

        Resource(const Resource& r) : my_handle(r.my_handle)
        {
            if(my_handle != Policy::null())
                CLPP_CHECK_ERROR(Policy::retain(my_handle));
        }

        void reset(Handle h)
        {
            if(my_handle != Policy::null() && my_handle != h)
                CLPP_CHECK_ERROR(Policy::release(my_handle));

            my_handle = h;
        }

        Handle operator*() const
        {
            return my_handle;
        }

        Resource& operator=(const Resource& r)
        {
            Resource<Handle> tmp(r);
            swap(tmp);
            return *this;
        }

        ~Resource() throw()
        {
            if(my_handle != Policy::null())
                Policy::release(my_handle);
        }

        void swap(Resource<Handle>& r) throw()
        {
            Handle tmp = my_handle;
            my_handle = r.my_handle;
            r.my_handle = tmp;
        }

    private:
        Handle my_handle;
}; // template <typename Handle> class Resource

} // namespace clpp

#endif // CLPP_RESOURCE_HPP
