cmake_minimum_required(VERSION 3.22)

project(cpeed VERSION 0.1.0 LANGUAGES CXX)

set(PACKAGE_STRING "${PROJECT_NAME}-${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(fmt CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

configure_file(
    ${PROJECT_SOURCE_DIR}/conf/config.hpp.in 
    ${PROJECT_SOURCE_DIR}/include/config.hpp
)