#ifndef CLPP_CONTEXT_HPP
#define CLPP_CONTEXT_HPP

#include <fstream>
#include <vector>


#include "platform.hpp"
#include "device.hpp"
#include "commandqueue.hpp"
#include "buffer.hpp"
#include "program.hpp"
#include "error.hpp"

namespace clpp {

class Context {
    public:
        Context(Platform platform = Platform(), cl_device_type type = CL_DEVICE_TYPE_DEFAULT)
            : my_devices(platform, type)
        {
            initByPlatform(platform.id(), type);
        }

        Context(DeviceList device_list) : my_devices(device_list)
        {
            initByDevices();
        }

        Context(Device device) : my_devices(device)
        {
            initByDevices();
        }

        cl_context id() const
        {
            return *my_resource;
        }

        Device device(size_t i = 0) const
        {
            return my_devices[i];
        }

        size_t getNumberOfDevices() const
        {
            return my_devices.size();
        }

        CommandQueue& queue(size_t i = 0)
        {
            return my_queues[i];
        }

        template <typename T> Buffer<T> createBuffer(size_t size, cl_mem_flags flags = CL_MEM_READ_WRITE, T* ptr = NULL)
        {
            cl_int err = 0;
            cl_mem mem = clCreateBuffer(context, flags, size*sizeof(T), ptr, &err);
            CLPP_CHECK_ERROR(err);
            return Buffer<T>(mem);
        }

        Program readProgramSource(const char* source, const char* options = NULL)
        {
            cl_int err;
            cl_program p = clCreateProgramWithSource(id(), 1, const_cast<const char**>(&source), NULL, &err);
            CLPP_CHECK_ERROR(err);
            err = clBuildProgram(p, my_devices.size(), my_devices.data(), options, NULL, NULL);
            CLPP_CHECK_ERROR(err);
            return Program(p);
        }

        Program readProgramSourceFile(const char* filename, const char* options = NULL)
        {
            std::ifstream fin(filename, std::ios::binary);
            fin.seekg(0, std::ios::end);
            std::vector<char> src(static_cast<size_t>(fin.tellg())+1, 0);
            fin.seekg(0, std::ios::beg);
            fin.read(&src[0], src.size());
            return readProgramSource(&src[0], options);
        }

        /*
        void checkProgram(Program p)
        {
            for(size_t i = 0; i < getNumberOfDevices(); ++i){
                Device d = device(i);
                if(p.status(d) != CL_BUILD_SUCCESS)
                    throw std::runtime_error("CL_BUILD_PROGRAM_FAILURE\n" + p.getBuildLog(d));
            }
        }
        */


    private:
        void initByPlatform(cl_platform_id platform, cl_device_type type)
        {
            cl_context_properties properties[] = {
                CL_CONTEXT_PLATFORM,
                reinterpret_cast<cl_context_properties>(platform),
                0
            };
            cl_int err;

            cl_context context = clCreateContextFromType(
                    properties,  // context properties
                    type,        // type of devices
                    NULL,        // error handling function
                    NULL,        // user data for error handling function
                    &err         // error code
            );
            
            CLPP_CHECK_ERROR(err);
            
            my_resource.reset(context);

            initQueues();
        }

        void initByDevices()
        {
            if(my_devices.size() == 0){
                // FIXME insert error handling here
            }
            
            cl_platform_id platform;
            clGetDeviceInfo(my_devices[0].id(), CL_DEVICE_PLATFORM, sizeof(cl_platform_id), &platform, NULL);
            
            cl_context_properties properties[] = {
                CL_CONTEXT_PLATFORM,
                reinterpret_cast<cl_context_properties>(platform),
                0
            };
            cl_int err;
            
            cl_context context = clCreateContext(
                    properties,        // context properties
                    my_devices.size(), // number of devices
                    my_devices.data(), // device IDs
                    NULL,              // error handling function
                    NULL,              // user data for error handling function
                    &err               // error code
            );
            
            CLPP_CHECK_ERROR(err);
            
            my_resource.reset(context);
            
            initQueues();
        }

        void initQueues()
        {
            cl_int err;
            for(size_t i = 0; i < my_devices.size(); ++i){
                cl_command_queue q = clCreateCommandQueue(id(), my_devices[i].id(), 0, &err);
                CLPP_CHECK_ERROR(err);
                my_queues.push_back(CommandQueue(q));
            }
        }

        Resource<cl_context> my_resource;
        DeviceList my_devices;
        std::vector<CommandQueue> my_queues;
};

} // namespace clpp

#endif // CLPP_CONTEXT_HPP
