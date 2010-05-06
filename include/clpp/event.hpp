//          Copyright Shan-Yung Yang 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLPP_EVENT_HPP
#define CLPP_EVENT_HPP

#include "resource.hpp"
#include "error.hpp"

namespace clpp {

class Event {
    public:
        cl_event id() const
        {
            return *my_resource;
        }

        cl_command_type type() const
        {
            return getInfo<cl_command_type>(CL_EVENT_COMMANT_TYPE);
        }

        cl_int status() const
        {
            return getInfo<cl_int>(CL_EVENT_COMMAND_EXECUTION_STATUS);
        }

        template <typename T> T getInfo(cl_event_info info)
        {
            T result;
            cl_int err = clGetEventInfo(id(), info, sizeof(T), &result, NULL);
            CLPP_CHECK_ERROR(err);
            return result;
        }

    private:
        Recource<cl_event> my_resource;
}; // class Event

} // namespace clpp

#endif // CLPP_EVENT_HPP
