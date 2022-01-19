# 被测试对象的目录
# set(SRC "${PROJECT_SOURCE_DIR}/src")
# set(SRC_HEADER "${PROJECT_SOURCE_DIR}/include")


# 将此目录编译成一个库供测试框架调用
# include_directories(${SRC_HEADER})
# add_library(cpeed_lib ${SRC_FILES})
# target_link_libraries(cpeed_lib PRIVATE cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_brotli_internal)
# target_include_directories(test_lib PRIVATE ${SRC_HEADER})
# set_target_properties(cpeed_lib PROPERTIES LINKER_LANGUAGE CXX)

# 设置输出路径
# SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/test")

# 测试工程目录配置
AUX_SOURCE_DIRECTORY("${PROJECT_SOURCE_DIR}/test" DIR_TESTS)
include_directories(include)

# 将测试工程目录下的所有测试文件编译成一个测试文件
add_executable(cpeed_test ${DIR_TESTS})
# target_include_directories(cpeed_test PRIVATE ${SRC_HEADER})
set_target_properties(cpeed_test PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(cpeed_test PRIVATE cpeed_lib)

# 被测试对象所使用的外部库
# find_package(cpprestsdk CONFIG REQUIRED)
# if(cpprestsdk_FOUND)
#     target_link_libraries(cpeed_test PRIVATE cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_brotli_internal)
#     message(STATUS "cpprestsdk_DIR = ${cpprestsdk_DIR}")
# endif()

# 找到测试框架并链接
find_package(GTest CONFIG REQUIRED)
if(GTest_FOUND)
    target_link_libraries(cpeed_test PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
    message(STATUS "GTest_DIR = ${GTest_DIR}")
endif()

# 启用测试框架
include(GoogleTest)
enable_testing()

#ADD_TEST(NAME cpeed_test COMMAND "${PROJECT_SOURCE_DIR}/bin/test/cpeed_test")
#set_tests_properties (cpeed_test PROPERTIES FAIL_REGULAR_EXPRESSION "failed")



