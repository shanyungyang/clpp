//          Copyright Shan-Yung Yang 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLPP_SIZE_HPP
#define CLPP_SIZE_HPP

namespace clpp {

class size2 {
    public:
        size2(size_t v = 0)
        {
            s[0] = s[1] = v;
        }

        size2(size_t v0, size_t v1)
        {
            s[0] = v0; s[1] = v1;
        }

        size_t s[2];
}; // class size2

class size3 {
    public:
        size3(size_t v = 0)
        {
            s[0] = s[1] = s[2] = v;
        }

        size3(size_t v0, size_t v1, size_t v2)
        {
            s[0] = v0; s[1] = v1; s[2] = v2;
        }

        size_t s[3];
}; // class size3

} // namespace clpp

#endif // CLPP_SIZE_HPP
