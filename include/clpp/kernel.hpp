//          Copyright Shan-Yung Yang 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLPP_KERNEL_HPP
#define CLPP_KERNEL_HPP

#include "resource.hpp"
#include "buffer.hpp"
#include "error.hpp"

namespace clpp {

/// The kernel object.
/** A kernel is a function declared in a program and executed on an OpenCL
    device. A kernel is identified by the \c __kernel or \c kernel qualifier
    applied to any function defined in a program.
 */
class Kernel {
    public:
        /// Construct a kernel object.
        /** Instead of using this constructor directly, please use
            Program::kernel to construct a kernel object.
         */
        Kernel(cl_kernel id = 0) : my_resource(id) {}

        /// Get the \c cl_kernel object created by OpenCL API.
        /**
            \return     the \c cl_kernel object created by OpenCL API.
         */
        cl_kernel id() const
        {
            return *my_resource;
        }

        /// Set arguments of this kernel function.
        template <typename T0>
        void setArgs(const T0& v0)
        {
            setArg(0, v0);
        }

        /// Set arguments of this kernel function.
        template <typename T0, typename T1>
        void setArgs(const T0& v0, const T1& v1)
        {
            setArg(1, v1);
            setArgs(v0);
        }

        /// Set arguments of this kernel function.
        template <typename T0, typename T1, typename T2>
        void setArgs(const T0& v0, const T1& v1, const T2& v2)
        {
            setArg(2, v2);
            setArgs(v0, v1);
        }

        /// Set arguments of this kernel function.
        template <typename T0, typename T1, typename T2, typename T3>
        void setArgs(const T0& v0, const T1& v1, const T2& v2, const T3& v3)
        {
            setArg(3, v3);
            setArgs(v0, v1, v2);
        }

        /// Set arguments of this kernel function.
        template <typename T0, typename T1, typename T2, typename T3, typename T4>
        void setArgs(const T0& v0, const T1& v1, const T2& v2, const T3& v3, const T4& v4)
        {
            setArg(4, v4);
            setArgs(v0, v1, v2, v3);
        }

        /// Set arguments of this kernel function.
        template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
        void setArgs(const T0& v0, const T1& v1, const T2& v2, const T3& v3, const T4& v4, const T5& v5)
        {
            setArg(5, v5);
            setArgs(v0, v1, v2, v3, v4);
        }

        /// Set arguments of this kernel function.
        template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
        void setArgs(const T0& v0, const T1& v1, const T2& v2, const T3& v3, const T4& v4, const T5& v5, const T6& v6)
        {
            setArg(6, v6);
            setArgs(v0, v1, v2, v3, v4, v5);
        }

        /// Set arguments of this kernel function.
        template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
        void setArgs(const T0& v0, const T1& v1, const T2& v2, const T3& v3, const T4& v4, const T5& v5, const T6& v6, const T7& v7)
        {
            setArg(7, v7);
            setArgs(v0, v1, v2, v3, v4, v5, v6);
        }

        /// Set a specific argument of this kernel function.
        /** 
            \tparam T           The type of the argument.
            \param arg_index    The index of specified argument.
            \param value        The argument value.
         */
        template <typename T> void setArg(cl_uint arg_index, const T& value)
        {
            cl_int err = clSetKernelArg(id(), arg_index, sizeof(T), &value);
            CLPP_CHECK_ERROR(err);
        }

        /// Set arguments of this kernel function.
        /** This function is a specialized version which is used for
            buffer objects.
         */
        template<typename T> void setArg(cl_uint arg_index, const Buffer<T>& memory)
        {
            cl_mem mem = memory.id();
            cl_int err = clSetKernelArg(id(), arg_index, sizeof(cl_mem), &mem);
            CLPP_CHECK_ERROR(err);
        }

    private:
        Resource<cl_kernel> my_resource;
}; // class Kernel

} // namespace clpp

#endif // CLPP_KERNEL_HPP
