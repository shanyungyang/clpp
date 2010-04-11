#ifndef CLPP_RESOURCE_HPP
#define CLPP_RESOURCE_HPP

#include <CL/cl.h>

namespace clpp {

template <typename Handle> struct ResourcePolicy;

template <> struct ResourcePolicy<cl_context> {
	static cl_context null() {	return 0; }
	static void retain(cl_context h) {	clRetainContext(h);	}
	static void release(cl_context h) { clReleaseContext(h); }
}; // struct ResourcePolicy<cl_context>

template <> struct ResourcePolicy<cl_command_queue> {
	static cl_command_queue null() { return 0; }
	static void retain(cl_command_queue h) { clRetainCommandQueue(h); }
	static void release(cl_command_queue h) { clReleaseCommandQueue(h); }
}; // struct ResourcePolicy<cl_command_queue>

template <> struct ResourcePolicy<cl_mem> {
	static cl_mem null() { return 0; }
	static void retain(cl_mem h) { clRetainMemObject(h); }
	static void release(cl_mem h) { clReleaseMemObject(h); }
}; // struct ResourcePolicy<cl_mem>

template <> struct ResourcePolicy<cl_program> {
	static cl_program null() { return 0; }
	static void retain(cl_program h) { clRetainProgram(h); }
	static void release(cl_program h) { clReleaseProgram(h); }
}; // struct ResourcePolicy<cl_program>

template <> struct ResourcePolicy<cl_kernel> {
	static cl_kernel null() { return 0; }
	static void retain(cl_kernel h) { clRetainKernel(h); }
	static void release(cl_kernel h) { clReleaseKernel(h); }
}; // struct ResourcePolicy<cl_kernel>

template <typename Handle> class Resource {
	public:
		typedef ResourcePolicy<Handle> Policy;

		Resource() : my_handle(Policy::null()) {}

		Resource(Handle h) : my_handle(h) {}

		Resource(const Resource& r) : my_handle(r.my_handle)
		{
			if(my_handle != Policy::null())
				Policy::retain(my_handle);
		}

		void reset(Handle h)
		{
			if(my_handle != Policy::null() && my_handle != h)
				Policy::release(my_handle);

			my_handle = h;
		}

		Handle operator*() const
		{
			return my_handle;
		}

		Resource& operator=(const Resource& r)
		{
			if(my_handle == *r)
				return *this; // silly self-assignment

			if(my_handle != Policy::null())
				Policy::release(my_handle);

			my_handle = r.my_handle;

			if(my_handle != Policy::null())
				Policy::retain(my_handle);

			return *this;
		}

		~Resource()
		{
			if(my_handle != Policy::null())
				Policy::release(my_handle);
		}

	private:
		Handle my_handle;
}; // template <typename Handle> class Resource

} // namespace clpp

#endif // CLPP_RESOURCE_HPP
