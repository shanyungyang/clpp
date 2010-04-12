#ifndef CLPP_PLATFORM_HPP
#define CLPP_PLATFORM_HPP

#include <vector>
#include <string>

#include "common.hpp"
#include "device.hpp"

namespace clpp {

/// The OpenCL platform.
class Platform {
	public:
		/// Construct the object as the first available platform on the system. 
		Platform()
		{
			clGetPlatformIDs(1, &my_id, 0);
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
			clGetPlatformInfo(my_id, info, 0, NULL, &len);
			std::string buf(len, 0);
			clGetPlatformInfo(my_id, info, len, &buf[0], NULL);
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

		/// Get number of devices on this platform with the specified device type.
		size_t getNumberOfDevices(cl_device_type type = CL_DEVICE_TYPE_DEFAULT)
		{
			cl_uint result;
			clGetDeviceIDs(my_id, type, 0, NULL, &result);
			return result;
		}

		/// Get the total number of available platforms on the system.
		static size_t getNumberOfPlatforms()
		{
			cl_uint result;
			clGetPlatformIDs(0, NULL, &result);
			return result;
		}

	private:
		cl_platform_id my_id;

}; // class Platform

} // namespace clpp

#endif // CLPP_PLATFORM_HPP
