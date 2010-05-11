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

/// The command queue object.
/** The command-queue is created on a specific device in a context and holds
    commands that will be executed. Commands to a command queue are queued
    in-order but may be executed in-order or out-of-order.
 */
class CommandQueue {
    public:
        /// Construct a command queue.
        /** Please use Context::createBuffer instead of using this constructor
            directly.
         */
        CommandQueue(cl_command_queue q) : my_resource(q) {}

        /// Get the \c cl_command_queue object created by OpenCL API.
        /**
            \return     The \c cl_command_queue object.
         */
        cl_command_queue id() const
        {
            return *my_resource;
        }

        /// Set execution mode of this command queue.
        /**
            \param mode     The desired execution mode, \a true for out-of-order
                            executation and \a false for in-order execution.
         */
        void setOutOfOrder(bool mode)
        {
            cl_bool enable = mode ? CL_TRUE : CL_FALSE;
            clSetCommandQueueProperty(id(), CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, enable, NULL);
        }

        /// Set profiling mode of this command queue.
        /**
            \param mode     The desired profiling mode, \a true for on and
                            \a false for off.
         */
        void setProfiling(bool mode)
        {
            cl_bool enable = mode ? CL_TRUE : CL_FALSE;
            clSetCommandQueueProperty(id(), CL_QUEUE_PROFILING_ENABLE, enable, NULL);
        }

        /// Enable out-of-order mode.
        /** This function directly calls setOutOfOrder(true).
         */
        void enableOutOfOrder()
        {
            setOutOfOrder(true);
        }

        /// Disable out-of-order mode.
        /** This function directly calls setOutOfOrder(true).
         */
        void disableOutOfOrder()
        {
            setOutOfOrder(false);
        }

        /// Enable profiling mode.
        /** This function directly calls setProfiling(true).
         */
        void enableProfiling()
        {
            setProfiling(true);
        }

        /// Disable profiling mode.
        /** This function directly calls setProfiling(false).
         */
        void disableProfiling()
        {
            setProfiling(false);
        }

        /// Flush the command queue.
        /** This function issues all previously queued OpenCL commands to the
            device associated with this command queue. There is \b no guarantee
            that all commands will be complete after this function returns.

            \sa finish()
         */
        void flush()
        {
            CLPP_CHECK_ERROR( clFlush(id()) );
        }

        /// Finish all jobs in the command queue.
        /** This function blocks until all previously queued commands are
            issued to the associated device and have completed.
         */
        void finish()
        {
            CLPP_CHECK_ERROR( clFinish(id()) );
        }

        /// Insert a barrier into the command queue.
        /** This function enqueues a barrier operation. The barrier operation
            is a command which ensures that all queued commands have finished
            execution before the next batch of commands can begin execution.
         */
        void barrier()
        {
            CLPP_CHECK_ERROR( clEnqueueBarrier(id()) );
        }

        /// Copy data from a buffer object to a host memory chunk.
        /**
            \param buffer   The buffer object where data are read.
            \param ptr      The host memory chunk where data are written.
            \param blocking Indicates if the read operations are blocking or
                            non-blocking. By default, blocking read is used.
         */
        template <typename T> void copy(const Buffer<T>& buffer, T* ptr, cl_bool blocking = CL_TRUE)
        {
            size_t cb = sizeof(T) * buffer.size();
            cl_int err = clEnqueueReadBuffer(id(), buffer.id(), blocking, 0, cb, ptr, 0, NULL, NULL);
            CLPP_CHECK_ERROR(err);
        }

        /// Copy a range of data from a buffer object to a host memory chunk.
        /**
            \param buffer   The buffer object where data are read.
            \param offset   The beginning index of items to be read.
            \param count    The number of items to be read.
            \param ptr      The host memory chunk where data are written.
            \param blocking Indicates if the read operations are blocking or
                            non-blocking. By default, blocking read is used.
         */
        template <typename T> void copy(const Buffer<T>& buffer, size_t offset, size_t count, T* ptr, cl_bool blocking = CL_TRUE)
        {
            cl_int err = clEnqueueReadBuffer(id(), buffer.id(), blocking, offset*sizeof(T), count*sizeof(T), ptr, 0, NULL, NULL);
            CLPP_CHECK_ERROR(err);
        }

        /// Copy data from a host memory chunk to a buffer object.
        /**
            \param buffer   The buffer object where data are written.
            \param ptr      The host memory chunk where data are read.
            \param blocking Indicates if the write operations are blocking or
                            non-blocking. By default, blocking write is used.
         */
        template <typename T> void copy(const T* ptr, const Buffer<T>& buffer, cl_bool blocking = CL_TRUE)
        {
            size_t cb = sizeof(T) * buffer.size();
            cl_int err = clEnqueueWriteBuffer(id(), buffer.id(), blocking, 0, cb, ptr, 0, NULL, NULL);
            CLPP_CHECK_ERROR(err);
        }

        /// Copy data from a host memory chunk to a range in a buffer object.
        /**
            \param buffer   The buffer object where data are written.
            \param offset   The beginning index of items to be written.
            \param count    The number of items to be written.
            \param ptr      The host memory chunk where data are read.
            \param blocking Indicates if the write operations are blocking or
                            non-blocking. By default, blocking write is used.
         */
        template <typename T> void copy(const T* ptr, const Buffer<T>& buffer, size_t offset, size_t count, cl_bool blocking = CL_TRUE)
        {
            cl_int err = clEnqueueWriteBuffer(id(), buffer.id(), blocking, offset*sizeof(T), count*sizeof(T), ptr, 0, NULL, NULL);
            CLPP_CHECK_ERROR(err);
        }

        /// Execute the kernel function.
        /** This function execute the specified kernel function by 1-D
            work-items.
            
            \param k            The specified kernel function.
            \param global_size  The number of global work-items.
            \param local_size   The number of work-items in a work-group.
                                If \a local_size is 0, an appropriate number
                                is determined by the OpenCL implementation.
         */
        void exec(Kernel k, size_t global_size, size_t local_size = 0)
        {
            cl_int err;
            if(local_size == 0)
                err = clEnqueueNDRangeKernel(id(), k.id(), 1, NULL, &global_size, NULL, 0, NULL, NULL);
            else
                err = clEnqueueNDRangeKernel(id(), k.id(), 1, NULL, &global_size, &local_size, 0, NULL, NULL);
            CLPP_CHECK_ERROR(err);
        }

        /// Execute the kernel function.
        /** This function execute the specified kernel function by 2-D
            work-items.
            
            \param k            The specified kernel function.
            \param global_size  The number of global work-items in each
                                dimension.
            \param local_size   The number of work-items in a work-group.
                                If any dimension of \a local_size is 0, an
                                appropriate number is determined by the OpenCL
                                implementation.
         */
        void exec(Kernel k, size2 global_size, size2 local_size = size2(0))
        {
            cl_int err;
            if(local_size.s[0] == 0 || local_size.s[1] == 0)
                err = clEnqueueNDRangeKernel(id(), k.id(), 2, NULL, global_size.s, NULL, 0, NULL, NULL);
            else
                err = clEnqueueNDRangeKernel(id(), k.id(), 2, NULL, global_size.s, local_size.s, 0, NULL, NULL);
            CLPP_CHECK_ERROR(err);
        }

        /// Execute the kernel function.
        /** This function execute the specified kernel function by 3-D
            work-items.
            
            \param k            The specified kernel function.
            \param global_size  The number of global work-items in each
                                dimension.
            \param local_size   The number of work-items in a work-group.
                                If any dimension of \a local_size is 0, an
                                appropriate number is determined by the OpenCL
                                implementation.
         */
        void exec(Kernel k, size3 global_size, size3 local_size = size3(0))
        {
            cl_int err;
            if(local_size.s[0] == 0 || local_size.s[1] == 0 || local_size.s[2] == 0)
                err = clEnqueueNDRangeKernel(id(), k.id(), 3, NULL, global_size.s, NULL, 0, NULL, NULL);
            else
                err = clEnqueueNDRangeKernel(id(), k.id(), 3, NULL, global_size.s, local_size.s, 0, NULL, NULL);
            CLPP_CHECK_ERROR(err);
        }

    private:
        Resource<cl_command_queue> my_resource;
}; // class CommandQueue

} // namespace clpp

#endif // CLPP_COMMANDQUEUE_HPP
