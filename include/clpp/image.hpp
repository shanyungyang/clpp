//          Copyright Shan-Yung Yang 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLPP_IMAGE_HPP
#define CLPP_IMAGE_HPP

#include "resource.hpp"
#include "buffer.hpp"
#include "size.hpp"

namespace clpp {

template <cl_channel_type T> struct ChannelType {};
template <> struct ChannelType<CL_SNORM_INT8>      { typedef cl_char   Type;};
template <> struct ChannelType<CL_SNORM_INT16>     { typedef cl_short  Type;};
template <> struct ChannelType<CL_UNORM_INT8>      { typedef cl_uchar  Type;};
template <> struct ChannelType<CL_UNORM_INT16>     { typedef cl_ushort Type;};
template <> struct ChannelType<CL_UNORM_SHORT_565> { typedef cl_short  Type;};
template <> struct ChannelType<CL_UNORM_SHORT_555> { typedef cl_short  Type;};
template <> struct ChannelType<CL_UNORM_INT_101010>{ typedef cl_int    Type;};
template <> struct ChannelType<CL_SIGNED_INT8>     { typedef cl_char   Type;};
template <> struct ChannelType<CL_SIGNED_INT16>    { typedef cl_short  Type;};
template <> struct ChannelType<CL_SIGNED_INT32>    { typedef cl_int    Type;};
template <> struct ChannelType<CL_UNSIGNED_INT8>   { typedef cl_uchar  Type;};
template <> struct ChannelType<CL_UNSIGNED_INT16>  { typedef cl_ushort Type;};
template <> struct ChannelType<CL_UNSIGNED_INT32>  { typedef cl_uint   Type;};
template <> struct ChannelType<CL_HALF_FLOAT>      { typedef cl_half   Type;};
template <> struct ChannelType<CL_FLOAT>           { typedef cl_float  Type;};

class Image : public Memory {
    public:
        /// Get image information.
        template <typename T> T getImageInfo(cl_image_info info) const
        {
            T result;
            cl_int err = clGetImageInfo(id(), info, sizeof(T), &result, NULL);
            CLPP_CHECK_ERROR(err);
            return result;
        }

        cl_image_format format() const
        {
            return getImageInfo<cl_image_format>(CL_IMAGE_FORMAT);
        }

        size_t width() const
        {
            return getImageInfo<size_t>(CL_IMAGE_WIDTH);
        }

        size_t height() const
        {
            return getImageInfo<size_t>(CL_IMAGE_HEIGHT);
        }

    protected:
        Image(cl_mem id) : Memory(id) {}
}; // class Image

class Image2D : public Image {
    public:
        Image2D(cl_mem id = 0) : Image(id) {}

        size2 size() const
        {
            size2 s;
            s.s[0] = width();
            s.s[1] = height();
            return s;
        }
}; // class Image2D


class Image3D : public Image {
    public:
        Image3D(cl_mem id = 0) : Image(id) {}

        size3 size()
        {
            size3 s;
            s.s[0] = width();
            s.s[1] = height();
            s.s[2] = depth();
            return s;
        }

        size_t depth() const
        {
            return getImageInfo<size_t>(CL_IMAGE_DEPTH);
        }

}; // class Image3D

} // namespace clpp

#endif // CLPP_IMAGE_HPP
