cmake_minimum_required(VERSION 3.6)
project(CoreJNI)

add_definitions(-DCORE_LOG_TAG="test")
set(CoreJNI_Root ${CMAKE_SOURCE_DIR}/../CoreJNI)
include_directories(
    ${CoreJNI_Root}/include
)

set(
    CoreJNI_FILES
    test.cpp
)

add_executable(corejni ${CoreJNI_FILES})
