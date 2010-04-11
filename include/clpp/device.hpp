#ifndef CLPP_DEVICE_HPP
#define CLPP_DEVICE_HPP

namespace clpp {

class Device {
	public:
		Device(cl_device_id id = 0) : my_id(id) {}

		std::string getInfo(cl_device_info info) const
		{
			size_t len;
			clGetDeviceInfo(my_id, info, 0, NULL, &len);
			std::vector<char> buf(len);
			clGetDeviceInfo(my_id, info, len, &buf[0], NULL);
			return std::string(&buf[0], len);
		}

		template <typename T> T getInfo(cl_device_info info) const
		{
			T result;
			clGetDeviceInfo(my_id, info, sizeof(T), &result, NULL);
			return result;
		}

		bool available() const
		{
			if(getInfo<cl_bool>(CL_DEVICE_AVAILABLE) == CL_TRUE)
				return true;
			else
				return false;
		}

		bool hasCompiler() const
		{
			if(getInfo<cl_bool>(CL_DEVICE_COMPILER_AVAILABLE) == CL_TRUE)
				return true;
			else
				return false;
		}

		std::string extensions() const
		{
			return getInfo(CL_DEVICE_EXTENSIONS);
		}

		std::string name() const
		{
			return getInfo(CL_DEVICE_NAME);
		}

		std::string vendor() const
		{
			return getInfo(CL_DEVICE_VENDOR);
		}

		std::string profile() const
		{
			return getInfo(CL_DEVICE_PROFILE);
		}

		cl_device_type type() const
		{
			return getInfo<cl_device_type>(CL_DEVICE_TYPE);
		}

		cl_device_id id() const
		{
			return my_id;
		}


	private:
		cl_device_id my_id;
}; // class Device

} // namespace clpp

#endif // CLPP_DEVICE_HPP
