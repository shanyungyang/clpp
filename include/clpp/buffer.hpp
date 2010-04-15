#ifndef CLPP_BUFFER_HPP
#define CLPP_BUFFER_HPP

#include "resource.hpp"
#include "context.hpp"

namespace clpp {

template <typename T> class Buffer {
    public:
        typedef T ValueType;

        Buffer(cl_mem id) : my_resource(id) {}

        cl_mem id() const
        {
            return *my_resource;
        }

        cl_mem_flags flags() const
        {
            cl_mem_flags f = 0;
            cl_int err = clGetMemObjectInfo(id(), CL_MEM_FLAGS, sizeof(cl_mem_flags), &f, NULL);
            CLPP_CHECK_ERROR(err);
            return f;
        }

        size_t size() const
        {
            size_t result = 0;
            clGetMemObjectInfo(id(), CL_MEM_SIZE, sizeof(size_t), &result, NULL);
            CLPP_CHECK_ERROR(err);
            return result / sizeof(T);
        }

    private:
        Resource<cl_mem> my_resource;
}; // template <typename T> class Buffer

} // namespace clpp

#endif // CLPP_BUFFER_HPP
