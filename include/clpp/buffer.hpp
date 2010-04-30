//          Copyright Shan-Yung Yang 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLPP_BUFFER_HPP
#define CLPP_BUFFER_HPP

#include "resource.hpp"
#include "context.hpp"

namespace clpp {


/// \brief The OpenCL buffer object.
///
/// Buffer is a memory object that stores a linear collection of bytes.
/// Buffer objects are accessible using a pointer in a kernel executing on a
/// device. A buffer object encapsulates the following information:
///   - Number of elements.
///   - Properties that describe usage information and which region to allocate
///     from.
///   - Buffer data.
template <typename T> class Buffer {
    public:
        typedef T ValueType;

        /// Construct a Buffer object.
        /// Please use Context::createBuffer instead of using this constructor directly.
        Buffer(cl_mem id) : my_resource(id) {}

        /// Get the cl_mem object created by OpenCL API.
        cl_mem id() const
        {
            return *my_resource;
        }

        /// Get the memory flags corresponding to this memory object.
        cl_mem_flags flags() const
        {
            cl_mem_flags f = 0;
            cl_int err = clGetMemObjectInfo(id(), CL_MEM_FLAGS, sizeof(cl_mem_flags), &f, NULL);
            CLPP_CHECK_ERROR(err);
            return f;
        }

        /// Get the number of elements in this memory object.
        size_t size() const
        {
            size_t result = 0;
            cl_int err = clGetMemObjectInfo(id(), CL_MEM_SIZE, sizeof(size_t), &result, NULL);
            CLPP_CHECK_ERROR(err);
            return result / sizeof(T);
        }

    private:
        Resource<cl_mem> my_resource;
}; // template <typename T> class Buffer

} // namespace clpp

#endif // CLPP_BUFFER_HPP
