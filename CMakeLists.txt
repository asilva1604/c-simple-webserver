cmake_minimum_required(VERSION 3.26)
project(c_simple_webserver C)

set(CMAKE_C_STANDARD 17)

add_executable(c_simple_webserver webserver.c)
