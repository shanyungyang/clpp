#ifndef CLPP_PLATFORM_HPP
#define CLPP_PLATFORM_HPP

#include <string>
#include <vector>
#include <stdexcept>

#include "common.hpp"
#include "error.hpp"
#include "device.hpp"

namespace clpp {

/// The OpenCL platform.
class Platform {
    public:
        /// Construct the object as the first available platform on the system. 
        Platform() : my_id(0)
        {
            cl_int err = clGetPlatformIDs(1, &my_id, 0);
            CLPP_CHECK_ERROR(err);
            // TODO we should use a more specific type for errors.
            if(my_id == 0)
                throw std::runtime_error("No available platform on this system");
        }

        /// Construct the object by the specified platform ID.
        Platform(cl_platform_id id) : my_id(id) {}

        /// Get the platform ID.
        cl_platform_id id() const
        {
            return my_id;
        }

        /// Get specific information about the platform.
        std::string getInfo(cl_platform_info info) const
        {
            size_t len;
            cl_int err = clGetPlatformInfo(my_id, info, 0, NULL, &len);
            CLPP_CHECK_ERROR(err);
            std::string buf(len, 0);
            err = clGetPlatformInfo(my_id, info, len, &buf[0], NULL);
            CLPP_CHECK_ERROR(err);
            return buf;
        }

        /// Get the OpenCL profile string.
        std::string profile() const
        {
            return getInfo(CL_PLATFORM_PROFILE);
        }

        /// Get the platform name string.
        std::string name() const
        {
            return getInfo(CL_PLATFORM_NAME);
        }

        /// Get the platform vendor string.
        std::string vendor() const
        {
            return getInfo(CL_PLATFORM_VENDOR);
        }

        /// Get the platform extension string.
        std::string extensions() const
        {
            return getInfo(CL_PLATFORM_EXTENSIONS);
        }

        /// Get the platform version string.
        std::string version() const
        {
            return getInfo(CL_PLATFORM_VERSION);
        }

    private:
        cl_platform_id my_id;

}; // class Platform

class PlatformList {
    public:
        PlatformList()
        {
            size_t num = 0;
            cl_int err = clGetPlatformIDs(0, NULL, &num);
            if(num == 0) // we need to check the number before the error code
                return;
            CLPP_CHECK_ERROR(err);
            my_list.resize(num);
            err = clGetPlatformIDs(num, &my_list[0], NULL);
            CLPP_CHECK_ERROR(err);
        }

        size_t size() const
        {
            return my_list.size();
        }

        Platform operator[](size_t i) const
        {
            return Platform(my_list[i]);
        }

    private:
        std::vector<cl_platform_id> my_list;
}; // class PlatformList

class DeviceList {
    public:
        DeviceList(Platform platform, cl_device_type type = CL_DEVICE_TYPE_DEFAULT)
        {
            size_t num = 0;
            cl_int err = clGetDeviceIDs(platform.id(), type, 0, NULL, &num);
            if(num == 0)
                return;
            CLPP_CHECK_ERROR(err);
            my_list.resize(num);
            err = clGetDeviceIDs(platform.id(), type, num, &my_list[0], NULL);
            CLPP_CHECK_ERROR(err);
        }

        /// Construct a empty device list
        DeviceList() {}

        /// Construct a device list which contains exactly one device
        DeviceList(Device d) : my_list(1, d.id()) {}

        void append(Device d)
        {
            my_list.push_back(d.id());
        }

        size_t size() const
        {
            return my_list.size();
        }

        Device operator[](size_t i) const
        {
            return Device(my_list[i]);
        }

        const cl_device_id* data() const
        {
            return &my_list[0];
        }

    private:
        std::vector<cl_device_id> my_list;
}; // class DeviceList

} // namespace clpp

#endif // CLPP_PLATFORM_HPP
