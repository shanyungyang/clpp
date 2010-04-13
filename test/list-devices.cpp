#include <iostream>
#include <iomanip>
#include <exception>
#include <string>
#include <vector>
#include <clpp/clpp.hpp>

using namespace std;
using namespace clpp;

inline const char* YesNo(bool f)
{
    return f ? "Yes" : "No";
}

void ListPlatformInfo(Platform p, size_t ind = 0)
{
    string indent(ind, ' ');
    cout << indent << "Platform:   " << p.name() << endl;
    cout << indent << "Vendor:     " << p.vendor() << endl;
    cout << indent << "Version:    " << p.version() << endl;
    cout << indent << "Profile:    " << p.profile() << endl;
    cout << indent << "Extensions: " << p.extensions() << endl;
}

void ListDeviceInfo(Device d, size_t ind = 0)
{
    string indent(ind, ' ');

    cout << indent << "Vendor:                   " << d.vendor() << endl;
    cout << indent << "Available:                " << YesNo( d.available() ) << endl;
    cout << indent << "Compiler:                 " << YesNo( d.hasCompiler() ) << endl;
    cout << indent << "Error correction:         " << YesNo( d.hasErrorCorrection() ) << endl;
    cout << indent << "Image support:            " << YesNo( d.supportImages() ) << endl;
    cout << indent << "Native kernel support:    " << YesNo( d.supportNativeKernels() ) << endl;

    size2 m2d = d.getImage2DMaxSize();
    size3 m3d = d.getImage3DMaxSize();

    cout << indent << "Maximum size of 2D image: " << m2d.s[0] << 'x' << m2d.s[1] << endl;
    cout << indent << "Maximum size of 3D image: " << m3d.s[0] << 'x' << m3d.s[1] << 'x' << m3d.s[2] << endl;

    cout << indent << "Number of cores:          " << d.getMaxComputeUnits() << endl;
    cout << indent << "Clock frequency:          " << d.getMaxClockFrequency() << endl;
    cout << indent << "Global memory size:       " << d.getGlobalMemorySize() << endl;
    cout << indent << "Local memory size:        " << d.getLocalMemorySize() << endl;
    cout << indent << "Maximum allocatable size: " << d.getMaxAllocatableSize() << endl;
    cout << indent << "Maximum parameter size:   " << d.getMaxParameterSize() << endl;

    cout << indent << "Maximum work-group size:  " << d.getMaxWorkGroupSize() << endl;

    // We currently don't implement an interface to retrieve maximum work-item size.
    cl_uint dim = d.getInfo<cl_uint>(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS);
    vector<size_t> max_work_size(dim);
    clGetDeviceInfo(d.id(), CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t)*dim, &max_work_size[0], NULL);
    cout << indent << "Maximum work-item size:   " << max_work_size[0];
    for(size_t i = 1; i < dim; ++i)
        cout << 'x' << max_work_size[i];
    cout << endl;
}

int main()
{
    try{
        PlatformList platform_list;

        for(size_t i = 0; i < platform_list.size(); ++i){
            Platform p = platform_list[i];
            ListPlatformInfo(p);
            cout << "Devices:" << endl;

            DeviceList device_list(p, CL_DEVICE_TYPE_ALL);
            for(size_t j = 0; j < device_list.size(); ++j){
                Device d = device_list[j];
                cout << setw(2) << j+1 << ". " << d.name() << endl;
                ListDeviceInfo(d, 4);
            }

        }
    }catch(const exception& err){
        cerr << "Fatal: " << err.what() << endl;
        return 1;
    }
    return 0;
}
