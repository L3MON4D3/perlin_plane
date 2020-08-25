cmake_minimum_required(VERSION 3.13)
project(worldWP CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED IMPORTED_TARGET glfw3)

add_subdirectory(libs/bgfx.cmake)

add_subdirectory(src)
