cmake_minimum_required(VERSION 3.5)

project(server VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 添加可执行文件
add_executable(server test.cpp utils.cpp server-new.cpp)