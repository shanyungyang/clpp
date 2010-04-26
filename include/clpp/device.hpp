#ifndef CLPP_DEVICE_HPP
#define CLPP_DEVICE_HPP

#include "error.hpp"
#include "vector.hpp"

namespace clpp {

/// The OpenCL device.
class Device {
    public:
        /// Construct the Device object by the specified device ID.
        Device(cl_device_id id) : my_id(id) {}

        /// Get the device ID.
        cl_device_id id() const
        {
            return my_id;
        }

        /// Get the device name.
        std::string name() const
        {
            return getInfo<std::string>(CL_DEVICE_NAME);
        }

        /// Get the vendor of the device.
        std::string vendor() const
        {
            return getInfo<std::string>(CL_DEVICE_VENDOR);
        }

        /// Get the version string of the device.
        std::string version() const
        {
            return getInfo<std::string>(CL_DEVICE_VERSION);
        }

        /// Get the profile string of the device.
        std::string profile() const
        {
            return getInfo<std::string>(CL_DEVICE_PROFILE);
        }

        /// Get the extension string of the device.
        std::string extensions() const
        {
            return getInfo<std::string>(CL_DEVICE_EXTENSIONS);
        }

        /// Get the device type.
        cl_device_type type() const
        {
            return getInfo<cl_device_type>(CL_DEVICE_TYPE);
        }

        /// Check if the device is available.
        bool available() const
        {
            return getInfo<cl_bool>(CL_DEVICE_AVAILABLE) == CL_TRUE;
        }

        /// Check if the device has the ability to compile device programs.
        bool hasCompiler() const
        {
            return getInfo<cl_bool>(CL_DEVICE_COMPILER_AVAILABLE) == CL_TRUE;
        }

        /// Check if the device support error correction.
        bool hasErrorCorrection() const
        {
            return getInfo<cl_bool>(CL_DEVICE_ERROR_CORRECTION_SUPPORT) == CL_TRUE;
        }

        /// Check if the device supports images
        bool hasImageSupport() const
        {
            return getInfo<cl_bool>(CL_DEVICE_IMAGE_SUPPORT) == CL_TRUE;
        }

        /// Check if the device are capable to execute native kernels.
        bool hasNativeKernelSupport()
        {
            return (getInfo<cl_device_exec_capabilities>(CL_DEVICE_EXECUTION_CAPABILITIES) & CL_EXEC_NATIVE_KERNEL) != 0;
        }

        /// Get the maximum size of a 2D image supported by the device.
        size2 getImage2DMaxSize() const
        {
            return size2( getInfo<size_t>(CL_DEVICE_IMAGE2D_MAX_WIDTH),
                          getInfo<size_t>(CL_DEVICE_IMAGE2D_MAX_HEIGHT) );
        }

        /// Get the maximum size of a 3D image supported by the device.
        size3 getImage3DMaxSize() const
        {
            return size3( getInfo<size_t>(CL_DEVICE_IMAGE3D_MAX_WIDTH),
                          getInfo<size_t>(CL_DEVICE_IMAGE3D_MAX_HEIGHT),
                          getInfo<size_t>(CL_DEVICE_IMAGE3D_MAX_DEPTH) );
        }

        /// Get the maximum clock frequency in MHz of this device.
        cl_uint getMaxClockFrequency() const
        {
            return getInfo<cl_uint>(CL_DEVICE_MAX_CLOCK_FREQUENCY);
        }

        /// The number of parallel compute cores on the device.
        cl_uint getMaxComputeUnits() const
        {
            return getInfo<cl_uint>(CL_DEVICE_MAX_COMPUTE_UNITS);
        }

        /// Get the size in bytes of global device memory.
        cl_ulong getGlobalMemSize() const
        {
            return getInfo<cl_ulong>(CL_DEVICE_GLOBAL_MEM_SIZE);
        }

        /// Get the size in bytes of local device memory.
        cl_ulong getLocalMemSize() const
        {
            return getInfo<cl_ulong>(CL_DEVICE_LOCAL_MEM_SIZE);
        }

        /// Get the maximum size in bytes of memory object allocation.
        cl_ulong getMaxMemAllocSize() const
        {
            return getInfo<cl_ulong>(CL_DEVICE_MAX_MEM_ALLOC_SIZE);
        }

        /// Get the maximum size in bytes of arguments that can be passed
        /// to kernels.
        size_t getMaxParameterSize() const
        {
            return getInfo<size_t>(CL_DEVICE_MAX_PARAMETER_SIZE);
        }

        /// Get the maximum number of work-items in a work-group.
        size_t getMaxWorkGroupSize() const
        {
            return getInfo<size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE);
        }

        /// Get the maximum number of work-items supported by this device.
        /// The returned value is an n-dimensional vector where n is the
        /// maximum dimension of work-items supported by the device, and
        /// each element represents the maximum number of work-items that
        /// can be specified in each dimension. The maximum number of
        /// work-items is the product of each element in this vector.
        std::vector<size_t> getMaxWorkItemSizes()
        {
            size_t dim = getInfo<cl_uint>(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS);
            std::vector<size_t> msize(dim);
            clGetDeviceInfo(id(), CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t)*dim, &msize[0], NULL);
            return msize;
        }

        /// Get device information by the specified type.
        template <typename T> T getInfo(cl_device_info info) const
        {
            T result;
            cl_int err = clGetDeviceInfo(my_id, info, sizeof(T), &result, NULL);
            CLPP_CHECK_ERROR(err);
            return result;
        }

        /// Get device information string.
        template <> std::string getInfo<std::string>(cl_device_info info) const
        {
            size_t len;
            cl_int err = clGetDeviceInfo(my_id, info, 0, NULL, &len);
            CLPP_CHECK_ERROR(err);
            std::string buf(len, 0);
            err = clGetDeviceInfo(my_id, info, len, &buf[0], NULL);
            CLPP_CHECK_ERROR(err);
            return buf;
        }

    private:
        cl_device_id my_id;
}; // class Device


} // namespace clpp

#endif // CLPP_DEVICE_HPP
