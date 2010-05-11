//          Copyright Shan-Yung Yang 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <string>
#include <clpp/clpp.hpp>

using namespace std;

// All classes and functions are declared under the clpp namespace.
using namespace clpp;

int main()
{
    try{
        // Initialize the OpenCL context. By default, the first available
        // device on the system is used.
        Context context;

        // A simple device program which compute squares.
        string src =
            "kernel void square(global int* output){"
            "    int i = get_global_id(0);"
            "    output[i] = i*i;"
            "}";
 
        // Construct the kernel object corresponding to the square()
        // function in the source.
        Kernel k = context.readProgramSource(src.c_str()).kernel("square");
 
        // Create a memory object which has 1024 integers.
        Buffer<cl_int> output = context.createBuffer<cl_int>(1024);

        // Set the argument of the kernel function so the output will
        // be written to the created memory object.
        k.setArgs(output);
 
        // A context may be associated with many devices, and each of
        // them has its own command queue. Calling Context::queue(i)
        // gives the command queue of the i'th device. If i is not
        // specified, the command queue of the first device is returned.
        CommandQueue q = context.queue();

        // Execute the kernel. Allocate 1024 work-items and group 64 work-
        // items into a work-group. (so there will be 16 work-groups.)
        q.exec(k, 1024, 64);
 
        // Read the output back to host memory.
        cl_int data[1024];
        q.copy(output, data);
 
        // check the answer
        cout << "Checking the answer..." << flush;
        bool failed = false;
        for(int i = 0; i < 16; ++i){
            if(data[i] != i*i){
                failed = true;
                break;
            }
        }
        
        if(failed){
            cout << "FAILED" << endl;
            cout << "Running kernel functions on your device results in wrong answers." << endl;
            cout << "Please check your driver and hardware configuration." << endl;
        }else{
            cout << "PASSED" << endl;
        }

    }catch(const Error& err){
        // All errors generated from OpenCL API are reported by exceptions.
        cerr << "Error code " << err.code() << ": " << err.what() << " in " << err.function() << endl;
        return 1;
    }
    return 0;
}
