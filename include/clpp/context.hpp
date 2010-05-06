//          Copyright Shan-Yung Yang 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLPP_CONTEXT_HPP
#define CLPP_CONTEXT_HPP

#include <fstream>
#include <vector>

#include "platform.hpp"
#include "device.hpp"
#include "commandqueue.hpp"
#include "buffer.hpp"
#include "image.hpp"
#include "program.hpp"
#include "error.hpp"

namespace clpp {

/// The OpenCL context.
/** By the definition from OpenCL specification, context is the environment
    within which the kernels execute and the domain in which synchronization
    and memory management is defined. The context includes a set of devices,
    the memory accessible to those devices, the corresponding memory properties
    and one or more command-queues used to schedule execution of a kernel(s)
    or operations on memory objects.
 */
class Context {
    public:
        /// Construct the context by the specified platform and device type.
        /** 
            \param platform The specified platform. By default, the first
                            available platform on the system is used.
            \param type     The type of devices to be used in the platform. By 
                            default, CL_DEVICE_TYPE_DEFAULT is used.
         */
        Context(Platform platform = Platform(), cl_device_type type = CL_DEVICE_TYPE_DEFAULT)
            : my_devices(platform, type)
        {
            initByPlatform(platform.id(), type);
        }

        /// Construct the context by a list of devices.
        /**
            \param device_list  The list of devices to be used in this context.
         */
        Context(DeviceList device_list) : my_devices(device_list)
        {
            initByDevices();
        }

        /// Construct the context by a specific device.
        /**
            \param device   The device to be used in this context.
         */
        Context(Device device) : my_devices(device)
        {
            initByDevices();
        }

        /// Get the cl_context object created by OpenCL API.
        /**
            \return     The \c cl_context object created by OpenCL API.
         */
        cl_context id() const
        {
            return *my_resource;
        }

        /// Get the list of devices associated with this context.
        /**
            \return     The \c DeviceList object containing all devices
                        associated with this context.
         */
        const DeviceList& devices() const
        {
            return my_devices;
        }

        /// Get the specific command queue associated with this context.
        /** If not specified, the first command queue will be returned.

            \param i    The number if specified command queue.

            \return     The \a i 'th command queue object in this context.
         */
        CommandQueue& queue(size_t i = 0)
        {
            return my_queues[i];
        }

        /// Create a buffer object.
        /**
            \tparam T       The type of element in this buffer object.
            \param size     Number of elements in this buffer object.
            \param flags    A bit-field that is used to specify allocation and
                            usage information such as the memory arena that
                            should be used to allocate the buffer object and
                            how it will be used.
            \param ptr      A pointer to the buffer data that may already be
                            allocated by the application. The buffer pointed
                            by \a host_ptr must contain equal or more than
                            \a size elements.

            \return         A buffer object which has \c size elements in type
                            \c T.
         */
        template <typename T>
        Buffer<T> createBuffer(size_t size, cl_mem_flags flags = CL_MEM_READ_WRITE, T* ptr = NULL)
        {
            cl_int err = 0;
            cl_mem mem = clCreateBuffer(id(), flags, size*sizeof(T), ptr, &err);
            CLPP_CHECK_ERROR(err);
            return Buffer<T>(mem);
        }

        /// Create a 2D image object.
        template <cl_channel_order O, cl_channel_type T>
        Image2D createImage(size2 size, cl_mem_flags flags = CL_MEM_READ_WRITE, typename ChannelType<T>::Type* host_ptr = NULL, size_t pitch = 0)
        {
            cl_image_format format;
            format.image_channel_order = O;
            format.image_channel_data_type = T;
            cl_int err;
            cl_mem mem = clCreateImage2D(id(), flags, &format, size.s[0], size.s[1], pitch, host_ptr, err);
            CLPP_CHECK_ERROR(err);
            return Image2D(mem);
        }

        /// Create a 3D image object.
        template <cl_channel_order O, cl_channel_type T>
        Image2D createImage(size3 size, cl_mem_flags flags = CL_MEM_READ_WRITE, typename ChannelType<T>::Type* host_ptr = NULL, size_t row_pitch = 0, size_t slice_pitch = 0)
        {
            cl_image_format format;
            format.image_channel_order = O;
            format.image_channel_data_type = T;
            cl_int err;
            cl_mem mem = clCreateImage3D(id(), flags, &format, size.s[0], size.s[1], size.s[2], row_pitch, slice_pitch, host_ptr, err);
            CLPP_CHECK_ERROR(err);
            return Image3D(mem);
        }

        /// Create a program object.
        /**
            \param source   The program source code. It must be null-terminated.
            \param options  Additional compiler options for this program.

            \return         The program object. The program is automatically
                            built for all devices associated with the context.
         */
        Program readProgramSource(const char* source, const char* options = NULL)
        {
            cl_int err;
            cl_program p = clCreateProgramWithSource(id(), 1, const_cast<const char**>(&source), NULL, &err);
            CLPP_CHECK_ERROR(err);
            err = clBuildProgram(p, my_devices.size(), my_devices.data(), options, NULL, NULL);
            CLPP_CHECK_ERROR(err);
            return Program(p);
        }

        /// Create a program object.
        /** This function create a program object from a source file.

            \param filename Name of the source file.
            \param options  Additional compiler options for this program.

            \return         The program object. The program is automatically
                            built for all devices associated with the context.
         */
        Program readProgramSourceFile(const char* filename, const char* options = NULL)
        {
            std::ifstream fin(filename, std::ios::binary);
            fin.seekg(0, std::ios::end);
            std::vector<char> src(static_cast<size_t>(fin.tellg())+1, 0);
            fin.seekg(0, std::ios::beg);
            fin.read(&src[0], src.size());
            return readProgramSource(&src[0], options);
        }

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
