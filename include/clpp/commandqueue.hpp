//          Copyright Shan-Yung Yang 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLPP_COMMANDQUEUE_HPP
#define CLPP_COMMANDQUEUE_HPP

#include "common.hpp"
#include "vector.hpp"
#include "kernel.hpp"
#include "resource.hpp"
#include "error.hpp"

namespace clpp {

class CommandQueue {
    public:
        CommandQueue(cl_command_queue q) : my_resource(q) {}

        /// Get the cl_command_queue object created by OpenCL API.
        cl_command_queue id() const
        {
            return *my_resource;
        }

        /// Set out-of-order mode of this command queue.
        void setOutOfOrder(bool flag)
        {
            cl_bool enable = flag ? CL_TRUE : CL_FALSE;
            clSetCommandQueueProperty(id(), CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, enable, NULL);
        }

        /// Set profiling mode of this command queue.
        void setProfiling(bool flag)
        {
            cl_bool enable = flag ? CL_TRUE : CL_FALSE;
            clSetCommandQueueProperty(id(), CL_QUEUE_PROFILING_ENABLE, enable, NULL);
        }

        /// Enable out-of-order mode. This function directly calls setOutOfOrder(true).
        void enableOutOfOrder()
        {
            setOutOfOrder(true);
        }

        /// Disable out-of-order mode. This function directly calls setOutOfOrder(true).
        void disableOutOfOrder()
        {
            setOutOfOrder(false);
        }

        /// Enable profiling mode. This function directly calls setProfiling(true).
        void enableProfiling()
        {
            setProfiling(true);
        }

        /// Disable profiling mode. This function directly calls setProfiling(false).
        void disableProfiling()
        {
            setProfiling(false);
        }

        /// Flush the command queue.
        void flush()
        {
            CLPP_CHECK_ERROR( clFlush(id()) );
        }

        /// Finish all jobs in the command queue.
        void finish()
        {
            CLPP_CHECK_ERROR( clFinish(id()) );
        }

        /// Insert a barrier into the command queue.
        void barrier()
        {
            CLPP_CHECK_ERROR( clEnqueueBarrier(id()) );
        }

        template <typename B> void read(const B& buffer, typename B::ValueType* ptr, cl_bool blocking = CL_TRUE)
        {
            size_t cb = sizeof(typename B::ValueType) * buffer.size();
            cl_int err = clEnqueueReadBuffer(id(), buffer.id(), blocking, 0, cb, ptr, 0, NULL, NULL);
            CLPP_CHECK_ERROR(err);
        }

        template <typename B> void read(const B& buffer, size_t offset, size_t cb, typename B::ValueType* ptr, cl_bool blocking = CL_TRUE)
        {
            typedef typename B::ValueType T;
            cl_int err = clEnqueueReadBuffer(id(), buffer.id(), blocking, offset*sizeof(T), cb*sizeof(T), ptr, 0, NULL, NULL);
            CLPP_CHECK_ERROR(err);
        }

        template <typename B> void write(const B& buffer, const typename B::ValueType* ptr, cl_bool blocking = CL_TRUE)
        {
            size_t cb = sizeof(typename B::ValueType) * buffer.size();
            cl_int err = clEnqueueWriteBuffer(id(), buffer.id(), blocking, 0, cb, ptr, 0, NULL, NULL);
            CLPP_CHECK_ERROR(err);
        }

        template <typename B> void write(const B& buffer, size_t offset, size_t cb, const typename B::ValueType* ptr, cl_bool blocking = CL_TRUE)
        {
            typedef typename B::ValueType T;
            cl_int err = clEnqueueWriteBuffer(id(), buffer.id(), blocking, offset*sizeof(T), cb*sizeof(T), ptr, 0, NULL, NULL);
            CLPP_CHECK_ERROR(err);
        }

        void exec(Kernel k, size_t global_size)
        {
            cl_int err = clEnqueueNDRangeKernel(id(), k.id(), 1, NULL, &global_size, NULL, 0, NULL, NULL);
            CLPP_CHECK_ERROR(err);
        }

        void exec(Kernel k, size_t global_size, size_t local_size)
        {
            cl_int err = clEnqueueNDRangeKernel(id(), k.id(), 1, NULL, &global_size, &local_size, 0, NULL, NULL);
            CLPP_CHECK_ERROR(err);
        }

        void exec(Kernel k, size2 global_size)
        {
            cl_int err = clEnqueueNDRangeKernel(id(), k.id(), 2, NULL, global_size.s, NULL, 0, NULL, NULL);
            CLPP_CHECK_ERROR(err);
        }

        void exec(Kernel k, size2 global_size, size2 local_size)
        {
            cl_int err = clEnqueueNDRangeKernel(id(), k.id(), 2, NULL, global_size.s, local_size.s, 0, NULL, NULL);
            CLPP_CHECK_ERROR(err);
        }

        void exec(Kernel k, size3 global_size)
        {
            cl_int err = clEnqueueNDRangeKernel(id(), k.id(), 3, NULL, global_size.s, NULL, 0, NULL, NULL);
            CLPP_CHECK_ERROR(err);
        }

        void exec(Kernel k, size3 global_size, size3 local_size)
        {
            cl_int err = clEnqueueNDRangeKernel(id(), k.id(), 3, NULL, global_size.s, local_size.s, 0, NULL, NULL);
            CLPP_CHECK_ERROR(err);
        }

    private:
        Resource<cl_command_queue> my_resource;
}; // class CommandQueue

} // namespace clpp

#endif // CLPP_COMMANDQUEUE_HPP
