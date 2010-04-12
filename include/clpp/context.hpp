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
        Context(cl_device_type type = CL_DEVICE_TYPE_DEFAULT)
        {
            Platform first_platform;
            init(first_platform.id(), type);
        }

        Context(Platform platform, cl_device_type type = CL_DEVICE_TYPE_DEFAULT)
        {
            init(platform.id(), type);
        }

        Context(Device device)
        {
            cl_device_id id = device.id();
            init(1, &id);
        }

        cl_context id() const
        {
            return *my_resource;
        }

        Device device(size_t i = 0) const
        {
            return Device(my_devices[i]);
        }

        size_t getNumberOfDevices() const
        {
            return my_devices.size();
        }

        CommandQueue& queue(size_t i = 0)
        {
            return my_queues[i];
        }

        template <typename T> Buffer<T> createBuffer(
                size_t size,
                cl_mem_flags flags = CL_MEM_READ_WRITE,
                T* ptr = NULL)
        {
            return Buffer<T>(id(), flags, size, ptr);
        }

        Program readProgramSource(const char* source, const char* options = NULL)
        {
            Program p(id(), source);
            cl_int err = clBuildProgram(p.id(), my_devices.size(), &my_devices[0], options, NULL, NULL);
            checkProgram(p);
            CheckError(err);
            return p;
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

        void checkProgram(Program p)
        {
            for(size_t i = 0; i < getNumberOfDevices(); ++i){
                Device d = device(i);
                if(p.status(d) != CL_BUILD_SUCCESS)
                    throw std::runtime_error("CL_BUILD_PROGRAM_FAILURE\n" + p.getBuildLog(d));
            }
        }


    private:
        void init(cl_platform_id platform, cl_device_type type)
        {
            cl_context_properties properties[] = {
                CL_CONTEXT_PLATFORM,
                reinterpret_cast<cl_context_properties>(platform),
                0
            };
            cl_int err;

            cl_context context = clCreateContextFromType(
                    properties,    // context properties
                    type,        // type of devices
                    NULL,         // error handling function
                    NULL,         // user data for error handling function
                    &err        // error code
            );
            
            CheckError(err);
            
            my_resource.reset(context);
            
            initDevicesAndQueues();
        }

        void init(int num_devices, const cl_device_id* devices)
        {
            if(num_devices == 0){
                // FIXME insert error handling here
            }
            
            cl_platform_id platform;
            clGetDeviceInfo(devices[0], CL_DEVICE_PLATFORM, sizeof(cl_platform_id), &platform, NULL);
            
            cl_context_properties properties[] = {
                CL_CONTEXT_PLATFORM,
                reinterpret_cast<cl_context_properties>(platform),
                0
            };
            cl_int err;
            
            cl_context context = clCreateContext(
                    properties,        // context properties
                    num_devices,    // number of devices
                    devices,        // device IDs
                    NULL,            // error handling function
                    NULL,            // user data for error handling function
                    &err            // error code
            );
            
            CheckError(err);
            
            my_resource.reset(context);
            
            initDevicesAndQueues();
        }

        void initDevicesAndQueues()
        {
            size_t devices_size = 0;
            clGetContextInfo(id(), CL_CONTEXT_DEVICES, 0, NULL, &devices_size);
            my_devices.resize(devices_size/sizeof(cl_device_id));
            clGetContextInfo(id(), CL_CONTEXT_DEVICES, devices_size, &my_devices[0], NULL);
            
            for(size_t i = 0; i < my_devices.size(); ++i)
                my_queues.push_back(CommandQueue(id(), my_devices[i]));
        }

        /*
        void onError(const char* errinfo, const void* private_info, size_t cb)
        {
            // FIXME error handling function
        }

        static void onError(const char* errinfo, const void* private_info, size_t cb, void* user_data)
        {
            reinterpret_cast<Context*>(user_data)->onError(errinfo, private_info, cb);
        }
        */

        Resource<cl_context> my_resource;

        std::vector<cl_device_id> my_devices;
        std::vector<CommandQueue> my_queues;
};

} // namespace clpp

#endif // CLPP_CONTEXT_HPP
