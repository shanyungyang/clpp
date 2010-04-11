#ifndef CLPP_PLATFORM_HPP
#define CLPP_PLATFORM_HPP

#include <vector>
#include <string>

#include "common.hpp"
#include "device.hpp"

namespace clpp {

class Platform {
	public:
		Platform(cl_platform_id id = 0) : my_id(id) {}

		cl_platform_id id() const
		{
			return my_id;
		}

		std::string getInfo(cl_platform_info info) const
		{
			size_t len;
			clGetPlatformInfo(my_id, info, 0, NULL, &len);
			std::vector<char> buf(len);
			clGetPlatformInfo(my_id, info, len, &buf[0], NULL);
			return std::string(&buf[0], len);
		}

		std::string profile() const
		{
			return getInfo(CL_PLATFORM_PROFILE);
		}

		std::string name() const
		{
			return getInfo(CL_PLATFORM_NAME);
		}

		std::string vendor() const
		{
			return getInfo(CL_PLATFORM_VENDOR);
		}

		std::string extensions() const
		{
			return getInfo(CL_PLATFORM_EXTENSIONS);
		}

		size_t getNumberOfDevices(cl_device_type type = CL_DEVICE_TYPE_DEFAULT)
		{
			cl_uint result;
			clGetDeviceIDs(my_id, type, 0, NULL, &result);
			return result;
		}

		Device getFirstDevice(cl_device_type type = CL_DEVICE_TYPE_DEFAULT)
		{
			cl_device_id device_id;
			clGetDeviceIDs(my_id, type, 1, &device_id, NULL);
			return Device(device_id);
		}

		static size_t getNumberOfPlatforms()
		{
			cl_uint result;
			clGetPlatformIDs(0, NULL, &result);
			return result;
		}

		// Get the first available platform
		static Platform getFirstPlatform()
		{
			Platform platform;
			clGetPlatformIDs(1, &platform.my_id, NULL);
			return platform;
		}

	private:
		cl_platform_id my_id;

}; // class Platform

} // namespace clpp

#endif // CLPP_PLATFORM_HPP
