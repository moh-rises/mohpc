cmake_minimum_required(VERSION 3.10)
project(MOHPC)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_DEPRECATE)
include_directories(include)

add_subdirectory(thirdparty)
add_subdirectory(mohpc-lib)
add_subdirectory(tests)
