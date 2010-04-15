#ifndef CLPP_KERNEL_HPP
#define CLPP_KERNEL_HPP

#include "resource.hpp"
#include "buffer.hpp"
#include "error.hpp"

namespace clpp {

class Kernel {
    public:
        Kernel(cl_kernel id) : my_resource(id) {}

        cl_kernel id() const
        {
            return *my_resource;
        }

        template <typename T0>
        void setArgs(const T0& v0)
        {
            setArg(0, v0);
        }

        template <typename T0, typename T1>
        void setArgs(const T0& v0, const T1& v1)
        {
            setArg(1, v1);
            setArgs(v0);
        }

        template <typename T0, typename T1, typename T2>
        void setArgs(const T0& v0, const T1& v1, const T2& v2)
        {
            setArg(2, v2);
            setArgs(v0, v1);
        }

        template <typename T0, typename T1, typename T2, typename T3>
        void setArgs(const T0& v0, const T1& v1, const T2& v2, const T3& v3)
        {
            setArg(3, v3);
            setArgs(v0, v1, v2);
        }

        template <typename T0, typename T1, typename T2, typename T3, typename T4>
        void setArgs(const T0& v0, const T1& v1, const T2& v2, const T3& v3, const T4& v4)
        {
            setArg(4, v4);
            setArgs(v0, v1, v2, v3);
        }

        template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
        void setArgs(const T0& v0, const T1& v1, const T2& v2, const T3& v3, const T4& v4, const T5& v5)
        {
            setArg(5, v5);
            setArgs(v0, v1, v2, v3, v4);
        }

        template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
        void setArgs(const T0& v0, const T1& v1, const T2& v2, const T3& v3, const T4& v4, const T5& v5, const T6& v6)
        {
            setArg(6, v6);
            setArgs(v0, v1, v2, v3, v4, v5);
        }

        template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
        void setArgs(const T0& v0, const T1& v1, const T2& v2, const T3& v3, const T4& v4, const T5& v5, const T6& v6, const T7& v7)
        {
            setArg(7, v7);
            setArgs(v0, v1, v2, v3, v4, v5, v6);
        }

        template <typename T> void setArg(cl_uint arg_index, const T& value)
        {
            cl_int err = clSetKernelArg(id(), arg_index, sizeof(T), &value);
            CLPP_CHECK_ERROR(err);
        }

        template <typename T> void setArg(cl_uint arg_index, const Buffer<T>& buffer)
        {
            cl_mem mem = buffer.id();
            cl_int err = clSetKernelArg(id(), arg_index, sizeof(cl_mem), &mem);
            CLPP_CHECK_ERROR(err);
        }

    private:
        Resource<cl_kernel> my_resource;
}; // class Kernel

} // namespace clpp

#endif // CLPP_KERNEL_HPP
