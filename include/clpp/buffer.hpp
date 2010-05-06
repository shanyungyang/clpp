//          Copyright Shan-Yung Yang 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLPP_BUFFER_HPP
#define CLPP_BUFFER_HPP

#include "resource.hpp"
#include "context.hpp"

namespace clpp {

/// The memory object.
/** The memory object holds a \c cl_mem object acquired from OpenCL.
    This class is used as the base class of Buffer and Image. 
 */
class Memory {
    public:
        /// Get the cl_mem object created by OpenCL API.
        cl_mem id() const
        {
            return *my_resource;
        }

        /// Get the memory flags corresponding to this memory object.
        /**
            \return     The \a flags argument value when the memory object is
                        created by Context::createBuffer.
         */
        cl_mem_flags flags() const
        {
            return getInfo<cl_mem_flags>(CL_MEM_FLAGS);
        }

        /// Get memory object information.
        /** This function returns information that is common to all memory
            objects including buffers and images.

            \tparam T       The type of returned information
            \param info     Specifies the information to query.

            \return         The queried value.
         */
        template <typename T> T getInfo(cl_mem_info info) const
        {
            T result;
            cl_int err = clGetMemObjectInfo(id(), info, sizeof(T), &result, NULL);
            CLPP_CHECK_ERROR(err);
            return result;
        }

        virtual ~Memory(){}

    protected:
        Memory(cl_mem id) : my_resource(id) {}

        Resource<cl_mem> my_resource;
}; // class Memory


/// \brief The OpenCL buffer object.
/** Buffer is a memory object that stores a linear collection of bytes.
    Buffer objects are accessible using a pointer in a kernel executing on a
    device. A buffer object encapsulates the following information:
      - Number of elements.
      - Properties that describe usage information and which region to allocate
        from.
      - Buffer data.
 */
template <typename T> class Buffer : public Memory {
    public:
        typedef T ValueType;

        /// Construct a Buffer object.
        /** Please use Context::createBuffer instead of using this constructor
            directly.
         */
        Buffer(cl_mem id) : Memory(id) {}

        /// Get the number of elements in this memory object.
        /**
            \return     Number of elements.
         */
        size_t size() const
        {
            return getInfo<size_t>(CL_MEM_SIZE) / sizeof(T);
        }
}; // template <typename T> class Buffer

} // namespace clpp

#endif // CLPP_BUFFER_HPP
