project (OpenGR-Apps-IOLib)

# Internal static library used for apps and tests

set(io_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB_RECURSE io_sources ${io_ROOT}/*.cc)
file(GLOB_RECURSE io_headers ${io_ROOT}/*.h ${io_ROOT}/*.hpp)

add_library(opengr_apps_io STATIC ${io_sources} ${io_headers})
target_include_directories(opengr_apps_io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

find_package(Filesystem QUIET)
if(CXX_FILESYSTEM_HAVE_FS)
    message(STATUS "[Deps] Found std::filesystem for image loading")
    target_compile_definitions(opengr_apps_io PUBLIC -DCXX_FILESYSTEM_HAVE_FS -DCXX_FILESYSTEM_IS_EXPERIMENTAL=$<BOOL:${CXX_FILESYSTEM_IS_EXPERIMENTAL}> -DCXX_FILESYSTEM_NAMESPACE=${CXX_FILESYSTEM_NAMESPACE})
    target_link_libraries(opengr_apps_io std::filesystem)
else ()
    message(STATUS "[Deps] std::filesystem not found")
endif(CXX_FILESYSTEM_HAVE_FS)

add_library(gr::io ALIAS opengr_apps_io)
