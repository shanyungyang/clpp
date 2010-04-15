#ifndef CLPP_PROGRAM_HPP
#define CLPP_PROGRAM_HPP

#include <fstream>
#include <vector>
#include <string>
#include "resource.hpp"
#include "kernel.hpp"
#include "error.hpp"

namespace clpp {

class Program {
    public:
        Program(cl_program p) : my_resource(p) {}

        cl_program id()
        {
            return *my_resource;
        }

        static void unloadCompiler()
        {
            clUnloadCompiler();
        }

        cl_build_status status(Device device)
        {
            cl_build_status result;
            clGetProgramBuildInfo(id(), device.id(), CL_PROGRAM_BUILD_STATUS, sizeof(result), &result, NULL);
            return result;
        }
    
        std::string getBuildLog(Device device)
        {
            size_t len;
            clGetProgramBuildInfo(id(), device.id(), CL_PROGRAM_BUILD_LOG, 0, NULL, &len);
            std::vector<char> buf(len);
            clGetProgramBuildInfo(id(), device.id(), CL_PROGRAM_BUILD_LOG, len, &buf[0], NULL);
            return std::string(&buf[0], len);
        }

        Kernel kernel(const char* kernel_name)
        {
            cl_int err = 0;
            cl_kernel k = clCreateKernel(id(), kernel_name, &err);
            CLPP_CHECK_ERROR(err);
            return Kernel(k);
        }

    private:
        Resource<cl_program> my_resource;
}; // class Program

} // namespace clpp

#endif // CLPP_PROGRAM_HPP
