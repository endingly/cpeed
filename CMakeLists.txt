cmake_minimum_required(VERSION 3.5)

# 设置 vcpkg
SET(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

project(cpeed VERSION 0.1.0)

# 设置编译器采用的标准版本  
set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works


# INCLUDE_DIRECTORIES(${ROOT_DIR}/src)
# 头文件包含目录
include_directories(include) 

aux_source_directory(src DIR_LIB_SRCS)

# 编译库版本供测试用
add_library(cpeed_lib ${DIR_LIB_SRCS})

# 二进制程序
add_executable(cpeed ${DIR_LIB_SRCS} main.cpp)

add_subdirectory(test)


# cpprestsdk
find_package(cpprestsdk CONFIG REQUIRED)
target_link_libraries(cpeed PRIVATE cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_brotli_internal)
target_link_libraries(cpeed_lib PRIVATE cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_brotli_internal)