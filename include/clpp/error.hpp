#ifndef CLPP_ERROR_HPP
#define CLPP_ERROR_HPP

#include <iostream>
#include <exception>
#include <string>
#include "common.hpp"

#ifdef __PRETTY_FUNCTION__
#undef __FUNCTION__
#define __FUNCTION__ __PRETTY_FUNCTION__
#endif

#ifndef __FUNCTION__
#if __STDC_VERSION__ >= 199901L // C99 supported
#define __FUNCTION__ __func__
#else
#define __FUNCTION__ "<UNKNOWN>"
#endif
#endif // #ifndef __FUNCTION__

namespace clpp {

#define CLPP_CODE_NAME(name) \
        case name: \
            return #name; \
            break;

inline const char* GetErrorCodeName(cl_int code)
{
    switch(code){
        CLPP_CODE_NAME(CL_BUILD_PROGRAM_FAILURE)
        CLPP_CODE_NAME(CL_DEVICE_NOT_AVAILABLE)
        CLPP_CODE_NAME(CL_DEVICE_NOT_FOUND)
        CLPP_CODE_NAME(CL_MAP_FAILURE)
        CLPP_CODE_NAME(CL_MEM_OBJECT_ALLOCATION_FAILURE)
        CLPP_CODE_NAME(CL_OUT_OF_HOST_MEMORY)
        CLPP_CODE_NAME(CL_OUT_OF_RESOURCES)

        CLPP_CODE_NAME(CL_IMAGE_FORMAT_MISMATCH)
        CLPP_CODE_NAME(CL_INVALID_ARG_INDEX)
        CLPP_CODE_NAME(CL_INVALID_ARG_SIZE)
        CLPP_CODE_NAME(CL_INVALID_ARG_VALUE)
        CLPP_CODE_NAME(CL_INVALID_BINARY)
        CLPP_CODE_NAME(CL_INVALID_BUFFER_SIZE)
        CLPP_CODE_NAME(CL_INVALID_BUILD_OPTIONS)
        CLPP_CODE_NAME(CL_INVALID_COMMAND_QUEUE)
        CLPP_CODE_NAME(CL_INVALID_CONTEXT)
        CLPP_CODE_NAME(CL_INVALID_DEVICE)
        CLPP_CODE_NAME(CL_INVALID_DEVICE_TYPE)
        CLPP_CODE_NAME(CL_INVALID_EVENT)
        CLPP_CODE_NAME(CL_INVALID_EVENT_WAIT_LIST)
        CLPP_CODE_NAME(CL_INVALID_GL_OBJECT)
        CLPP_CODE_NAME(CL_INVALID_GLOBAL_OFFSET)
        CLPP_CODE_NAME(CL_INVALID_HOST_PTR)
        CLPP_CODE_NAME(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR)
        CLPP_CODE_NAME(CL_INVALID_IMAGE_SIZE)
        CLPP_CODE_NAME(CL_INVALID_KERNEL_NAME)
        CLPP_CODE_NAME(CL_INVALID_KERNEL)
        CLPP_CODE_NAME(CL_INVALID_KERNEL_ARGS)
        CLPP_CODE_NAME(CL_INVALID_KERNEL_DEFINITION)
        CLPP_CODE_NAME(CL_INVALID_PLATFORM)
        CLPP_CODE_NAME(CL_INVALID_PROGRAM)
        CLPP_CODE_NAME(CL_INVALID_PROGRAM_EXECUTABLE)
        CLPP_CODE_NAME(CL_INVALID_QUEUE_PROPERTIES)
        CLPP_CODE_NAME(CL_INVALID_SAMPLER)
        CLPP_CODE_NAME(CL_INVALID_VALUE)
        CLPP_CODE_NAME(CL_INVALID_WORK_DIMENSION)
        CLPP_CODE_NAME(CL_INVALID_WORK_ITEM_SIZE)

        CLPP_CODE_NAME(CL_COMPILER_NOT_AVAILABLE)
        CLPP_CODE_NAME(CL_IMAGE_FORMAT_NOT_SUPPORTED)
        CLPP_CODE_NAME(CL_INVALID_OPERATION)
        CLPP_CODE_NAME(CL_MEM_COPY_OVERLAP)
        CLPP_CODE_NAME(CL_PROFILING_INFO_NOT_AVAILABLE)

        default:
            return "UNKNOWN";
    }
} // const char* GetErrorCodeName(cl_int)

#undef CLPP_CODE_NAME

class Error : public std::exception {
    public:
        Error(cl_int code, const char* filename, const char* func, size_t line)
            : my_code(code), my_filename(filename),
              my_function(func), my_line(line)
        {}

        virtual const char* what() const throw()
        {
            return GetErrorCodeName(my_code);
        }

        cl_int code() const throw()
        {
            return my_code;
        }

        const char* filename() const throw()
        {
            return my_filename.c_str();
        }

        const char* function() const throw()
        {
            return my_function.c_str();
        }

        size_t line() const
        {
            return my_line;
        }

        virtual ~Error()
        {}

    private:
        cl_int my_code;
        std::string my_filename;
        std::string my_function;
        size_t my_line;
};

inline void CheckError(cl_int code, const char* filename, const char* function, size_t line)
{
    if(code != CL_SUCCESS)
        throw Error(code, filename, function, line);
}

#define CLPP_CHECK_ERROR(code) CheckError(code, __FILE__, __FUNCTION__, __LINE__)

} // namespace clpp

#endif // CLPP_ERROR_HPP
