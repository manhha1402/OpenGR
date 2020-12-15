project(OpenGR-externalAppTest)
cmake_minimum_required(VERSION 3.15)

set (CMAKE_CXX_STANDARD 11)

find_package(OpenGR REQUIRED)
find_package( Eigen3 REQUIRED )
add_executable(${PROJECT_NAME} main.cpp)
include_directories( ${EIGEN3_INCLUDE_DIR} )
target_link_libraries(${PROJECT_NAME} gr::algo gr::utils)

install( TARGETS ${PROJECT_NAME} RUNTIME DESTINATION . )


