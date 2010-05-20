//          Copyright Shan-Yung Yang 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLPP_EVENT_HPP
#define CLPP_EVENT_HPP

#include "resource.hpp"
#include "error.hpp"

namespace clpp {

/// The event object.
/** An event object encapsulates the status of an operation such as a command.
    It can be used to synchronize operations in a context or get profiling
    information.
 */
class Event {
    public:
        /// Construct an event objct.
        /** Don't use this constructor directly. Use member functions in
            CommandQueue to obtain event objects.
         */
        Event(cl_event id = 0) : my_resource(id)
        {}

        /// Get the \c cl_event object created by the OpenCL API.
        /**
            \return     The \c cl_event object created by the OpenCL API.
         */
        cl_event id() const
        {
            return *my_resource;
        }

        /// Get the type of command identified by this event object.
        /**
            \return     The type of command. It can be one of the following
                        values:
                            - CL_COMMAND_NDRANGE_KERNEL
                            - CL_COMMAND_TASK
                            - CL_COMMAND_NATIVE_KERNEL
                            - CL_COMMAND_READ_BUFFER
                            - CL_COMMAND_WRITE_BUFFER
                            - CL_COMMAND_COPY_BUFFER
                            - CL_COMMAND_READ_IMAGE
                            - CL_COMMAND_WRITE_IMAGE
                            - CL_COMMAND_COPY_IMAGE
                            - CL_COMMAND_COPY_BUFFER_TO_IMAGE
                            - CL_COMMAND_COPY_IMAGE_TO_BUFFER
                            - CL_COMMAND_MAP_BUFFER
                            - CL_COMMAND_MAP_IMAGE
                            - CL_COMMAND_UNMAP_MEM_OBJECT
                            - CL_COMMAND_MARKER
                            - CL_COMMAND_ACQUIRE_GL_OBJECTS
                            - CL_COMMAND_RELEASE_GL_OBJECTS
         */
        cl_command_type type() const
        {
            return getInfo<cl_command_type>(CL_EVENT_COMMAND_TYPE);
        }

        /// Get the execution status of the command identified by this event object.
        /**
            \return     The execution status. It can be one of the following
                        values:
                            - CL_QUEUED
                            - CL_SUBMITTED
                            - CL_RUNNING
                            - CL_COMPLETE
         */
        cl_int status() const
        {
            return getInfo<cl_int>(CL_EVENT_COMMAND_EXECUTION_STATUS);
        }

        /// Get the device time counter when the command is enqueued.
        /**
            \return     The device time counter in nanosecond when the command
                        is enqueued in a command queue.
         */
        cl_ulong getQueuedTime() const
        {
            return getProfilingInfo<cl_ulong>(CL_PROFILING_COMMAND_QUEUED);
        }

        /// Get the device time counter when the command is submitted.
        /**
            \return     The device time counter in nanosecond when the command
                        is submitted to the device associated with the command
                        queue.
         */
        cl_ulong getSubmitTime() const
        {
            return getProfilingInfo<cl_ulong>(CL_PROFILING_COMMAND_SUBMIT);
        }

        /// Get the device time counter when the command starts execution.
        /**
            \return     The device time counter in nanosecond when the command
                        starts execution on the device.
         */
        cl_ulong getStartTime() const
        {
            return getProfilingInfo<cl_ulong>(CL_PROFILING_COMMAND_START);
        }

        /// Get the device time counter when the command has finished execution.
        /**
            \return     The device time counter in nanosecond when the command
                        has finished execution on the device.
         */
        cl_ulong getEndTime() const
        {
            return getProfilingInfo<cl_ulong>(CL_PROFILING_COMMAND_END);
        }

        /// Get the execution time of the command identified by this event object.
        /**
            \return     The execution time. i.e. \c getStartTime() - \c getEndTime()
         */
        cl_ulong getExecutionTime() const
        {
            return getEndTime() - getStartTime();
        }

        template <typename T> T getInfo(cl_event_info info) const
        {
            T result;
            cl_int err = clGetEventInfo(id(), info, sizeof(T), &result, NULL);
            CLPP_CHECK_ERROR(err);
            return result;
        }

        template <typename T> T getProfilingInfo(cl_profiling_info info) const
        {
            T result;
            cl_int err = clGetEventProfilingInfo(id(), info, sizeof(T), &result, NULL);
            CLPP_CHECK_ERROR(err);
            return result;
        }

        /// Wait until the command identified by this event object has finished execution.
        void wait()
        {
            cl_event e = id();
            clWaitForEvents(1, &e);
        }

    private:
        Resource<cl_event> my_resource;
}; // class Event

} // namespace clpp

#endif // CLPP_EVENT_HPP
