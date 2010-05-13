//          Copyright Shan-Yung Yang 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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
/**
    Platform is the host plus a collection of devices managed by the OpenCL
    framework that allow an application to share resources and execute kernels
    on devices in the platform.
 */
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
        /** 
            \param id   The platform ID obtained by OpenCL API.
         */
        Platform(cl_platform_id id) : my_id(id) {}

        /// Get the platform ID.
        /**
            \return     The platform ID used in OpenCL API.
         */
        cl_platform_id id() const
        {
            return my_id;
        }

        /// Get specific information about the platform.
        /**
            \param info An enumeration constant that identifies the platform
                        information being queried.

            \return     The specified information, in std::string.
         */
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
        /** This function actually calls Platform::getInfo(CL_PLATFORM_PROFILE).

            \return     The profile name supported by the implementation.
         */
        std::string profile() const
        {
            return getInfo(CL_PLATFORM_PROFILE);
        }

        /// Get the platform name string.
        /** This function actually calls Platform::getInfo(CL_PLATFORM_NAME).

            \return     The name string of the platform.
         */
        std::string name() const
        {
            return getInfo(CL_PLATFORM_NAME);
        }

        /// Get the platform vendor string.
        /** This function actually calls Platform::getInfo(CL_PLATFORM_VENDOR).

            \return     The vendor string of the platform.
         */
        std::string vendor() const
        {
            return getInfo(CL_PLATFORM_VENDOR);
        }

        /// Get the platform extension string.
        /** This function actually calls Platform::getInfo(CL_PLATFORM_EXTENSIONS).

            \return     A space-separated list of extension names (the
                        extension names themselves do not contain any spaces)
                        supported by the platform.
         */
        std::string extensions() const
        {
            return getInfo(CL_PLATFORM_EXTENSIONS);
        }

        /// Get the platform version string.
        /** This function actually calls Platform::getInfo(CL_PLATFORM_VERSION).

            \return     The OpenCL version supported by the implementation.
         */
        std::string version() const
        {
            return getInfo(CL_PLATFORM_VERSION);
        }

    private:
        cl_platform_id my_id;

}; // class Platform

/// A list of platforms.
class PlatformList {
    public:
        /// Construct the list of available platforms on the system.
        PlatformList()
        {
            cl_uint num = 0;
            cl_int err = clGetPlatformIDs(0, NULL, &num);
            if(num == 0) // we need to check the number before the error code
                return;
            CLPP_CHECK_ERROR(err);
            my_list.resize(num);
            err = clGetPlatformIDs(num, &my_list[0], NULL);
            CLPP_CHECK_ERROR(err);
        }

        /// Get the size of the platform list.
        /**
            \return     Number of platforms in this list.
         */
        size_t size() const
        {
            return my_list.size();
        }

        /// Get the specific platform in this list.
        /**
            \param i    The number of the specified platform.

            \return     The \a i 'th platform in this list.
         */
        Platform operator[](size_t i) const
        {
            return Platform(my_list[i]);
        }

    private:
        std::vector<cl_platform_id> my_list;
}; // class PlatformList

/// A list of devices.
class DeviceList {
    public:
        /// Construct the device list which belong to the specified device type, under the given platform.
        /**
            \param platform The platform which all devices in this list should
                            be associated with.
            \param type     The type which all devices in this list should
                            belong to. By default, CL_DEVICE_TYPE_DEFAULT is
                            specified.
         */
        DeviceList(Platform platform, cl_device_type type = CL_DEVICE_TYPE_DEFAULT)
        {
            cl_uint num = 0;
            cl_int err = clGetDeviceIDs(platform.id(), type, 0, NULL, &num);
            if(num == 0) // check the number before error code
                return;
            CLPP_CHECK_ERROR(err);
            my_list.resize(num);
            err = clGetDeviceIDs(platform.id(), type, num, &my_list[0], NULL);
            CLPP_CHECK_ERROR(err);
        }

        /// Construct an empty device list.
        DeviceList() {}

        /// Construct the device list which contains exactly one device.
        /**
            \param d    The device which is contained in this list.
         */
        DeviceList(Device d) : my_list(1, d.id()) {}

        /// Append a device to this device list.
        /**
            \param d    The device to be appended.
         */
        void append(Device d)
        {
            my_list.push_back(d.id());
        }

        /// Check if these devices are associated with the same platform.
        /**
            \return     true if all devices in this list are associated with
                        the same platform, and false if not.
         */
        bool underSamePlatform() const
        {
            if(my_list.size() == 0)
                return false;

            cl_platform_id p0, pi;
            cl_int err = clGetDeviceInfo(my_list[0], CL_DEVICE_PLATFORM, sizeof(cl_platform_id), &p0, NULL);
            CLPP_CHECK_ERROR(err);
            for(size_t i = 1; i < my_list.size(); ++i){
                err = clGetDeviceInfo(my_list[0], CL_DEVICE_PLATFORM, sizeof(cl_platform_id), &pi, NULL);
                CLPP_CHECK_ERROR(err);
                if(p0 != pi)
                    return false;
            }
            return true;
        }

        /// Get the number of devices in this list.
        /**
            \return Number of devices.
         */
        size_t size() const
        {
            return my_list.size();
        }

        /// Get the specific device in this list.
        /**
            \param i    The number of the specified device.
            \return     The \a i 'th device in this list.
         */
        Device operator[](size_t i) const
        {
            return Device(my_list[i]);
        }

        /// Get the raw pointer to cl_device_id in this list.
        /** Clients should not modify the content pointed by this function.
         
            \return     The pointer to the list of <tt>cl_device_id</tt>s,
                        which can be used in OpenCL API.
         */
        const cl_device_id* data() const
        {
            return &my_list[0];
        }

    private:
        std::vector<cl_device_id> my_list;
}; // class DeviceList

} // namespace clpp

#endif // CLPP_PLATFORM_HPP
