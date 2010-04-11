#ifndef CLPP_COMMANDQUEUE_HPP
#define CLPP_COMMANDQUEUE_HPP

#include "common.hpp"
#include "vector.hpp"
#include "kernel.hpp"
#include "resource.hpp"
#include "error.hpp"

namespace clpp {

class CommandQueue {
	public:
		cl_command_queue id() const
		{
			return *my_resource;
		}

		void setOutOfOrder(bool flag)
		{
			cl_bool enable = flag ? CL_TRUE : CL_FALSE;
			clSetCommandQueueProperty(id(), CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, enable, NULL);
		}

		void setProfiling(bool flag)
		{
			cl_bool enable = flag ? CL_TRUE : CL_FALSE;
			clSetCommandQueueProperty(id(), CL_QUEUE_PROFILING_ENABLE, enable, NULL);
		}

		void enableOutOfOrder()
		{
			setOutOfOrder(true);
		}

		void disableOutOfOrder()
		{
			setOutOfOrder(false);
		}

		void enableProfiling()
		{
			setProfiling(true);
		}

		void disableProfiling()
		{
			setProfiling(false);
		}

		void flush()
		{
			CheckError( clFlush(id()) );
		}

		void finish()
		{
			CheckError( clFinish(id()) );
		}

		void barrier()
		{
			CheckError( clEnqueueBarrier(id()) );
		}

		template <typename B> void read(const B& buffer, typename B::ValueType* ptr, cl_bool blocking = CL_TRUE)
		{
			size_t cb = sizeof(typename B::ValueType) * buffer.size();
			cl_int err = clEnqueueReadBuffer(id(), buffer.id(), blocking, 0, cb, ptr, 0, NULL, NULL);
			CheckError(err);
		}

		template <typename B> void read(const B& buffer, size_t offset, size_t cb, typename B::ValueType* ptr, cl_bool blocking = CL_TRUE)
		{
			typedef typename B::ValueType T;
			cl_int err = clEnqueueReadBuffer(id(), buffer.id(), blocking, offset*sizeof(T), cb*sizeof(T), ptr, 0, NULL, NULL);
			CheckError(err);
		}

		template <typename B> void write(const B& buffer, const typename B::ValueType* ptr, cl_bool blocking = CL_TRUE)
		{
			size_t cb = sizeof(typename B::ValueType) * buffer.size();
			cl_int err = clEnqueueWriteBuffer(id(), buffer.id(), blocking, 0, cb, ptr, 0, NULL, NULL);
			CheckError(err);
		}

		template <typename B> void write(const B& buffer, size_t offset, size_t cb, const typename B::ValueType* ptr, cl_bool blocking = CL_TRUE)
		{
			typedef typename B::ValueType T;
			cl_int err = clEnqueueWriteBuffer(id(), buffer.id(), blocking, offset*sizeof(T), cb*sizeof(T), ptr, 0, NULL, NULL);
			CheckError(err);
		}

		void exec(Kernel k, size_t global_size)
		{
			cl_int err = clEnqueueNDRangeKernel(id(), k.id(), 1, NULL, &global_size, NULL, 0, NULL, NULL);
			CheckError(err);
		}

		void exec(Kernel k, size_t global_size, size_t local_size)
		{
			cl_int err = clEnqueueNDRangeKernel(id(), k.id(), 1, NULL, &global_size, &local_size, 0, NULL, NULL);
			CheckError(err);
		}

		void exec(Kernel k, size2 global_size)
		{
			cl_int err = clEnqueueNDRangeKernel(id(), k.id(), 2, NULL, global_size.s, NULL, 0, NULL, NULL);
			CheckError(err);
		}

		void exec(Kernel k, size2 global_size, size2 local_size)
		{
			cl_int err = clEnqueueNDRangeKernel(id(), k.id(), 2, NULL, global_size.s, local_size.s, 0, NULL, NULL);
			CheckError(err);
		}

		void exec(Kernel k, size3 global_size)
		{
			cl_int err = clEnqueueNDRangeKernel(id(), k.id(), 3, NULL, global_size.s, NULL, 0, NULL, NULL);
			CheckError(err);
		}

		void exec(Kernel k, size3 global_size, size3 local_size)
		{
			cl_int err = clEnqueueNDRangeKernel(id(), k.id(), 3, NULL, global_size.s, local_size.s, 0, NULL, NULL);
			CheckError(err);
		}

	private:
		friend class Context;

		CommandQueue(cl_context context, cl_device_id device)
		{
			cl_int err;
			cl_command_queue queue = clCreateCommandQueue(context, device, 0, &err);
			CheckError(err);
			my_resource.reset(queue);
		}
		Resource<cl_command_queue> my_resource;
}; // class CommandQueue

} // namespace clpp

#endif // CLPP_COMMANDQUEUE_HPP
