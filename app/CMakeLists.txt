# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

########################################################################################
# Need C++11 (required by dlib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

########################################################################################
# Prebuilt log lib,  included in the NDK.

find_library(log-lib
        log
    )


########################################################################################
# set up external include and lib, such as OpenCL headers
set(EXTERNAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../external/include)
include_directories(${EXTERNAL_INCLUDE_DIR})

set(EXTERNAL_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../external/lib)

########################################################################################
# Set up NEON build flag
# [This example only build for armeabi-v7a, x86 could be done the same way]
#
if (${ANDROID_ABI} STREQUAL "armeabi-v7a")
    add_definitions("-DDLIB_HAVE_NEON")
    add_definitions("-DHAVE_NEON")
    add_definitions("-mfpu=neon")
endif ()

########################################################################################
# set up OpenCL headers and libs

# OpenCL headers in ${EXTERNAL_INCLUDE_DIR}/CL.

add_library( OpenCL SHARED IMPORTED )
set_target_properties(OpenCL PROPERTIES IMPORTED_LOCATION ${EXTERNAL_LIB_DIR}/${ANDROID_ABI}/libOpenCL.so)

find_library( opencl-lib OpenCL )


########################################################################################
# set up OpenCV

set(OpenCV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../external/OpenCV-android-sdk/sdk/native/jni)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# The lib files at ${OpenCV_LIBRARIES}

add_library( lib_opencv SHARED IMPORTED )
set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${OpenCV_DIR}/../libs/${ANDROID_ABI}/libopencv_java3.so)

########################################################################################
# Build dlib as static lib

set(DLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../dlib)

include_directories(${DLIB_DIR})

add_library(dlib STATIC
        ${DLIB_DIR}/dlib/threads/threads_kernel_shared.cpp
        ${DLIB_DIR}/dlib/entropy_decoder/entropy_decoder_kernel_2.cpp
        ${DLIB_DIR}/dlib/base64/base64_kernel_1.cpp
        ${DLIB_DIR}/dlib/threads/threads_kernel_1.cpp
        ${DLIB_DIR}/dlib/threads/threads_kernel_2.cpp
        ${DLIB_DIR}/dlib/data_io/mnist.cpp
        ${DLIB_DIR}/dlib/dnn/tensor_tools.cpp
        ${DLIB_DIR}/dlib/dnn/cpu_dlib.cpp
        ${DLIB_DIR}/dlib/threads/threaded_object_extension.cpp
        ${DLIB_DIR}/dlib/threads/thread_pool_extension.cpp
    )


########################################################################################
# Build JNI dlib test driver with face_landmark_detection_ex.cpp

add_library(face_land_test SHARED
        src/main/cpp/face_landmark_detection_ex.cpp
    )

target_link_libraries(face_land_test
        ${OpenCV_LIBRARIES}
        dlib
    )

add_library(face_recog_test SHARED
        src/main/cpp/dnn_face_recognition_ex.cpp
    )

target_link_libraries(face_recog_test
        ${OpenCV_LIBRARIES}
        dlib
    )


########################################################################################
# Build OpenCL 2.0 test driver with testcl.cpp

add_library(testcl SHARED src/main/cpp/testcl.cpp)

target_link_libraries( testcl
        ${log-lib}
        OpenCL
    )



########################################################################################
# Test with OpenCL, OpenCV, and GLES

add_library(clprocessor SHARED
        src/main/cpp/CLprocessor.cpp
        src/main/cpp/jni.cpp
        )

target_link_libraries ( clprocessor
        ${log-lib}
        OpenCL
        ${OpenCV_LIBRARIES}
        EGL
        GLESv2
        lib_opencv
    )


########################################################################################
# Build target JNI lib as shared, for Java invocation
add_library( native-lib  SHARED

        src/main/cpp/native-lib.cpp
    )


target_link_libraries( native-lib

        ${log-lib}
        ${OpenCV_LIBRARIES}
        face_land_test
        face_recog_test
        testcl
        dlib
    )

