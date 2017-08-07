#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_TARGET_OPENCL_VERSION 200

#include <iostream>
#include <algorithm>

#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl2.hpp>
#else
#include <CL/cl2.hpp>
#endif

#include <android/log.h>
#define app_name "testcl"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, app_name, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, app_name, __VA_ARGS__))

int test_vecadd()
{
    // Filter for a 2.0 platform and set it as the default
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);

    if(all_platforms.size()==0){
        LOGE(" No platforms found. Check OpenCL installation!\n");
        return -1;
    }

    cl::Platform default_platform=all_platforms[0];
    LOGI("Using platform: %s\n", (default_platform.getInfo<CL_PLATFORM_NAME>()).c_str());

    //get default device of the default platform
    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);

    if(all_devices.size()==0){
        LOGE(" No devices found. Check OpenCL installation!\n");
        return -1;
    }

    cl::Device default_device=all_devices[0];
    LOGI("Using device: %s\n", (default_device.getInfo<CL_DEVICE_NAME>()).c_str());

    cl::Context context({default_device});

    cl::Program::Sources sources;

    // kernel calculates for each element C=A+B
    std::string kernel_code=
            " void kernel simple_add(global const int* A, global const int* B, global int* C){ "
                    " int i = get_global_id(0);"
                    " C[i]=A[i]+B[i]; "
                    " } ";

    sources.push_back({kernel_code.c_str(),kernel_code.length()});

    cl::Program program(context,sources);

    if(program.build({default_device})!=CL_SUCCESS){
        LOGE(" Error building: %s\n", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device).c_str());
        return -1;
    }

    // create buffers on the device
    cl::Buffer buffer_A(context,CL_MEM_READ_WRITE,sizeof(int)*10);
    cl::Buffer buffer_B(context,CL_MEM_READ_WRITE,sizeof(int)*10);
    cl::Buffer buffer_C(context,CL_MEM_READ_WRITE,sizeof(int)*10);

    int A[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int B[] = {0, 1, 2, 0, 1, 2, 0, 1, 2, 0};

    //create queue to which we will push commands for the device.
    cl::CommandQueue queue(context,default_device);

    //write arrays A and B to the device
    queue.enqueueWriteBuffer(buffer_A,CL_TRUE,0,sizeof(int)*10,A);
    queue.enqueueWriteBuffer(buffer_B,CL_TRUE,0,sizeof(int)*10,B);

    //run the kernel
    //	cl::KernelFunctor simple_add(cl::Kernel(program,"simple_add"),queue,cl::NullRange,cl::NDRange(10),cl::NullRange);
    // simple_add(buffer_A,buffer_B,buffer_C);

    //alternative way to run the kernel
    cl::Kernel kernel_add=cl::Kernel(program,"simple_add");
    kernel_add.setArg(0,buffer_A);
    kernel_add.setArg(1,buffer_B);
    kernel_add.setArg(2,buffer_C);

    queue.enqueueNDRangeKernel(kernel_add,cl::NullRange,cl::NDRange(10),cl::NullRange);
    queue.finish();

    int C[10];
    //read result C from the device to array C
    queue.enqueueReadBuffer(buffer_C,CL_TRUE,0,sizeof(int)*10,C);

    for(int i=0;i<10;i++){
        LOGI("Result: C[%d] = %d ",i,C[i]);
        if(C[i] != A[i]+B[i]) {
            return -1;
        }
    }
    return 1;
}
