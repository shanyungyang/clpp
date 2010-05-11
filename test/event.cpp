//          Copyright Shan-Yung Yang 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <clpp/clpp.hpp>

using namespace std;
using namespace clpp;

// This is a short example demostrating how event object can be used.
int main()
{
    try{
        // A simple device program containing an empty loop.
        string src =
            "kernel void main(){"
            "    for(int i = 0; i < 1000000; ++i); "
            "}";

        Context context;
        Kernel k = context.readProgramSource(src.c_str()).kernel("main");

        CommandQueue q = context.queue();
        q.enableProfiling();

        Event e = q.exec(k, 4096);
        e.wait(); // Wait until this command has been finished.
        cout << "Total execution time: " << e.getExecutionTime() << "ns" << endl;

    }catch(const Error& err){
        cerr << "Error code " << err.code() << ": " << err.what() << " in " << err.function() << endl;
        return 1;
    }
    return 0;
}
