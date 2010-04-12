#ifndef CLPP_BUFFER_HPP
#define CLPP_BUFFER_HPP

#include "resource.hpp"

namespace clpp {

template <typename T> class Buffer {
    public:
        typedef T ValueType;

        cl_mem id() const
        {
            return *my_resource;
        }

        size_t size() const
        {
            if(id() == 0)
                return 0;

            size_t result;
            clGetMemObjectInfo(id(), CL_MEM_SIZE, sizeof(size_t), &result, NULL);
            return result / sizeof(T);
        }

    private:
        friend class Context;

        Buffer(cl_context context, cl_mem_flags flags, size_t size, T* ptr = NULL)
        {
            cl_int err;
            cl_mem mem = clCreateBuffer(context, flags, size*sizeof(T), ptr, &err);
            CheckError(err);
            my_resource.reset(mem);
        }

        Resource<cl_mem> my_resource;

}; // template <typename T> class Buffer

} // namespace clpp

#endif // CLPP_BUFFER_HPP
