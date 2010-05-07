The clpp project provides another C++ binding for OpenCL parallel programming
API.

The features of clpp include:
  - Clpp is a header-only library. It is very easy to install and use clpp in
    your project.
  - Clean and type-safe interface. We encapsulate pointers into classes as much
    as possible.
  - Constructors of OpenCL vector types are also provided.
  - Errors are reported by exceptions.

To use clpp, simply copy the entire include/clpp directory to your include
path and configure your project so that OpenCL.lib (or libOpenCL.so on Linux)
will be linked.

In the test/ directory you can find two programs:
  - list-device     This program will list all platforms and devices on your
                    system, with some additional information.
  - example         This is a short example demonstrating how to use clpp.

Please see BUGS.txt for known bugs or bug reporting process.


For more information about OpenCL, see http://www.khronos.org/opencl/.
