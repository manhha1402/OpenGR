cmake_minimum_required (VERSION 3.3)
project (OpenGR-Apps LANGUAGES CXX)

add_subdirectory(io)


find_package(OpenMP)
set(OpenGRAppsDeps)
if(OpenMP_CXX_FOUND)
    set(OpenGRAppsDeps OpenMP::OpenMP_CXX)
    message(STATUS "Parallelization enabled using OpenMP")
endif()


if(OpenGR_COMPILE_APPS)
    add_subdirectory(Super4PCS)
    add_subdirectory(PCLWrapper)
    add_subdirectory(ExtPointBinding)
endif(OpenGR_COMPILE_APPS)
