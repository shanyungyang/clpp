//          Copyright Shan-Yung Yang 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLPP_SIZE_HPP
#define CLPP_SIZE_HPP

namespace clpp {

/// A 2D size object.
/** This class encapsulates two numbers in \c size_t. It is useful to
    specify a two-dimensional size for images or work-items.
 */
class size2 {
    public:
        /// Construct a 2D size.
        /**
            \param v    The initial sizes in both dimensions.
         */
        size2(size_t v = 0)
        {
            s[0] = s[1] = v;
        }

        /// Construct a 2D size.
        /**
            \param v0   The specified size in the first dimension.
            \param v1   The specified size in the second dimension.
         */
        size2(size_t v0, size_t v1)
        {
            s[0] = v0; s[1] = v1;
        }

        /// Values in each dimension.
        size_t s[2];
}; // class size2

/// A 3D size object.
/** This class encapsulates three numbers in \c size_t. It is useful to
    specify a three-dimensional size for images or work-items.
 */
class size3 {
    public:
        /// Construct a 3D size.
        /**
            \param v    The initial sizes in all dimensions.
         */
        size3(size_t v = 0)
        {
            s[0] = s[1] = s[2] = v;
        }

        /// Construct a 2D size.
        /**
            \param v0   The specified size in the first dimension.
            \param v1   The specified size in the second dimension.
            \param v2   The specified size in the third dimension.
         */
        size3(size_t v0, size_t v1, size_t v2)
        {
            s[0] = v0; s[1] = v1; s[2] = v2;
        }

        /// Values in each dimension.
        size_t s[3];
}; // class size3

} // namespace clpp

#endif // CLPP_SIZE_HPP
