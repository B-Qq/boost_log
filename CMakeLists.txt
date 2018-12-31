cmake_minimum_required(VERSION 3.12.3)
project(Test)
set(CMAKE_CXX_STANDARD  11) #设置c++版本
set(BOOST_MIN_VERSION "1.69.0") #设置boost版本
FIND_PACKAGE(Boost ${BOOST_MIN_VERSION} REQUIRED)#查找boost目录
if (NOT Boost_FOUND)
    message("no find")
endif()
include_directories(${PROJECT_SOURCE_DIR}/incldue)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})#将boost头文件目录包含到工程中
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})#将boost库目录包含到工程中
add_definitions("-DBOOST_LOG_DYN_LINK")#添加编译选项

add_executable(test test.cpp Hello.cpp log.cpp)#生成可执行文件的名字

target_link_libraries(test boost_log_setup boost_log boost_system boost_thread boost_filesystem)#链接库
