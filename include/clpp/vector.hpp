#ifndef CLPP_VECTOR_HPP
#define CLPP_VECTOR_HPP

#include "common.hpp"

namespace clpp {

#define CLPP_GEN_VECTOR_CTOR_2(base) \
inline cl_##base##2 base##2(cl_##base s0, cl_##base s1) \
{ \
	cl_##base##2 r; r.s[0] = s0; r.s[1] = s1; \
	return r; \
} \
inline cl_##base##2 base##2(cl_##base s) \
{ \
	cl_##base##2 r; r.s[0] = r.s[1] = s; \
	return r; \
}

#define CLPP_GEN_VECTOR_CTOR_4(base) \
inline cl_##base##4 base##4(cl_##base s0, cl_##base s1, cl_##base s2, cl_##base s3) \
{ \
	cl_##base##4 r; r.s[0] = s0; r.s[1] = s1; r.s[2] = s2; r.s[3] = s3; \
	return r; \
} \
inline cl_##base##4 base##4(cl_##base s) \
{ \
	cl_##base##4 r; r.s[0] = r.s[1] = r.s[2] = r.s[3] = s; \
	return r; \
}

#define CLPP_GEN_VECTOR_CTOR_8(base) \
inline cl_##base##8 base##8(cl_##base s0, cl_##base s1, cl_##base s2, cl_##base s3, \
		                    cl_##base s4, cl_##base s5, cl_##base s6, cl_##base s7) \
{ \
	cl_##base##8 r; \
	r.s[0] = s0; r.s[1] = s1; r.s[2] = s2; r.s[3] = s3; \
	r.s[4] = s4; r.s[5] = s5; r.s[6] = s6; r.s[7] = s7; \
	return r; \
} \
inline cl_##base##8 base##8(cl_##base s) \
{ \
	cl_##base##8 r; \
	r.s[0] = r.s[1] = r.s[2] = r.s[3] = s; \
	r.s[4] = r.s[5] = r.s[6] = r.s[7] = s; \
	return r; \
}

#define CLPP_GEN_VECTOR_CTOR_16(base) \
inline cl_##base##16 base##16(cl_##base s0, cl_##base s1, cl_##base s2, cl_##base s3, \
		                      cl_##base s4, cl_##base s5, cl_##base s6, cl_##base s7, \
		                      cl_##base s8, cl_##base s9, cl_##base s10, cl_##base s11, \
		                      cl_##base s12, cl_##base s13, cl_##base s14, cl_##base s15) \
{ \
	cl_##base##16 r; \
	r.s[0] = s0; r.s[1] = s1; r.s[2] = s2; r.s[3] = s3; \
	r.s[4] = s4; r.s[5] = s5; r.s[6] = s6; r.s[7] = s7; \
	r.s[8] = s8; r.s[9] = s9; r.s[10] = s10; r.s[11] = s11; \
	r.s[12] = s12; r.s[13] = s13; r.s[14] = s14; r.s[15] = s15; \
	return r; \
} \
inline cl_##base##16 base##16(cl_##base s) \
{ \
	cl_##base##16 r; \
	r.s[0] = r.s[1] = r.s[2] = r.s[3] = s; \
	r.s[4] = r.s[5] = r.s[6] = r.s[7] = s; \
	r.s[8] = r.s[9] = r.s[10] = r.s[11] = s; \
	r.s[12] = r.s[13] = r.s[14] = r.s[15] = s; \
	return r; \
}

#define CLPP_GEN_VECTOR_CTOR(base) \
CLPP_GEN_VECTOR_CTOR_2(base) \
CLPP_GEN_VECTOR_CTOR_4(base) \
CLPP_GEN_VECTOR_CTOR_8(base) \
CLPP_GEN_VECTOR_CTOR_16(base) \


CLPP_GEN_VECTOR_CTOR(char)
CLPP_GEN_VECTOR_CTOR(uchar)
CLPP_GEN_VECTOR_CTOR(short)
CLPP_GEN_VECTOR_CTOR(ushort)
CLPP_GEN_VECTOR_CTOR(int)
CLPP_GEN_VECTOR_CTOR(uint)
CLPP_GEN_VECTOR_CTOR(long)
CLPP_GEN_VECTOR_CTOR(ulong)
CLPP_GEN_VECTOR_CTOR(float)
CLPP_GEN_VECTOR_CTOR(double)

#undef CLPP_GEN_VECTOR_CTOR_2
#undef CLPP_GEN_VECTOR_CTOR_4
#undef CLPP_GEN_VECTOR_CTOR_8
#undef CLPP_GEN_VECTOR_CTOR_16
#undef CLPP_GEN_VECTOR_CTOR

class size2 {
	public:
		size2(size_t x, size_t y)
		{
			s[0] = x; s[1] = y;
		}

		size_t s[2];
}; // class size2

class size3 {
	public:
		size3(size_t x, size_t y, size_t z)
		{
			s[0] = x; s[1] = y; s[2] = z;
		}

		size_t s[2];
}; // class size3



} // namespace clpp

#endif // CLPP_VECTOR_HPP
