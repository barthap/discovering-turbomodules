cmake_minimum_required(VERSION 3.4.1)

set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 14)

set(build_DIR ${CMAKE_SOURCE_DIR}/build)
set(PACKAGE_NAME "my_turbo_utils")  # the name for our library

# Specifies a path to native header files.
include_directories(
        ../cpp
)


# find fbjni package
find_package(fbjni REQUIRED CONFIG)

# create library "my_turbo_utils"
add_library(${PACKAGE_NAME}
        SHARED
        ../cpp/TurboUtilsModule.cpp
        cpp-adapter.cpp
)

# link fbjni and logger to my_turbo_utils
target_link_libraries(${PACKAGE_NAME} fbjni::fbjni android log)
