//          Copyright Shan-Yung Yang 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <clpp/clpp.hpp>

using namespace std;
using namespace clpp;

// This is a short test to display compiler error messages.
int main(){
    try{
		// A program containing errors.
        const char src[] = "kernel void main(){ output[i] = i; }";

        Context context;
        Program p = context.readProgramSource(src);
    }catch(const Program::BuildError& err){
		// Build error
		cout << "Build error catched successfully with error message:" << endl;
		cout << err.log() << endl;
		return 0;
	}catch(const Error& err){
		// other kinds of error
        cerr << "Error code " << err.code() << ": " << err.what() << " in " << err.function() << endl;
		return 1;
    }
    return 0;
}
