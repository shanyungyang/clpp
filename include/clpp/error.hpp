#ifndef CLPP_ERROR_HPP
#define CLPP_ERROR_HPP

#include <exception>
#include "common.hpp"

namespace clpp {

class Error : public std::exception {
    public:
        Error(cl_int code, const char* str) 
            : std::exception(str), my_code(code)
        {}

        cl_int code()
        {
            return my_code;
        }

    private:
        cl_int my_code;
}; // class Error

#define CLPP_CHECK_RUNTIME_ERROR(name) \
    case name: \
        throw std::runtime_error(#name); \
        break;

#define CLPP_CHECK_INVALID_ARGUMENT_ERROR(name) \
    case name: \
        throw std::invalid_argument(#name); \
        break;

#define CLPP_CHECK_LOGIC_ERROR(name) \
    case name: \
        throw std::logic_error(#name); \
        break;

inline void CheckError(cl_int code)
{
    if(code == CL_SUCCESS)
        return;

    switch(code){
        // CLPP_CHECK_RUNTIME_ERROR(CL_BUILD_PROGRAM_FAILURE)
        CLPP_CHECK_RUNTIME_ERROR(CL_DEVICE_NOT_AVAILABLE)
        CLPP_CHECK_RUNTIME_ERROR(CL_DEVICE_NOT_FOUND)
        CLPP_CHECK_RUNTIME_ERROR(CL_MAP_FAILURE)
        CLPP_CHECK_RUNTIME_ERROR(CL_MEM_OBJECT_ALLOCATION_FAILURE)
        CLPP_CHECK_RUNTIME_ERROR(CL_OUT_OF_HOST_MEMORY)
        CLPP_CHECK_RUNTIME_ERROR(CL_OUT_OF_RESOURCES)

        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_IMAGE_FORMAT_MISMATCH)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_ARG_INDEX)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_ARG_SIZE)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_ARG_VALUE)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_BINARY)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_BUFFER_SIZE)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_BUILD_OPTIONS)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_COMMAND_QUEUE)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_CONTEXT)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_DEVICE)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_DEVICE_TYPE)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_EVENT)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_EVENT_WAIT_LIST)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_GL_OBJECT)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_GLOBAL_OFFSET)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_HOST_PTR)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_IMAGE_SIZE)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_KERNEL_NAME)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_KERNEL)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_KERNEL_ARGS)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_KERNEL_DEFINITION)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_PLATFORM)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_PROGRAM)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_PROGRAM_EXECUTABLE)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_QUEUE_PROPERTIES)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_SAMPLER)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_VALUE)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_WORK_DIMENSION)
        CLPP_CHECK_INVALID_ARGUMENT_ERROR(CL_INVALID_WORK_ITEM_SIZE)

        CLPP_CHECK_LOGIC_ERROR(CL_COMPILER_NOT_AVAILABLE)
        CLPP_CHECK_LOGIC_ERROR(CL_IMAGE_FORMAT_NOT_SUPPORTED)
        CLPP_CHECK_LOGIC_ERROR(CL_INVALID_OPERATION)
        CLPP_CHECK_LOGIC_ERROR(CL_MEM_COPY_OVERLAP)
        CLPP_CHECK_LOGIC_ERROR(CL_PROFILING_INFO_NOT_AVAILABLE)

        default:
            throw std::runtime_error("UNKNOWN");
    };
}


} // namespace clpp

#endif // CLPP_ERROR_HPP
