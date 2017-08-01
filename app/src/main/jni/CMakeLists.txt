cmake_minimum_required(VERSION 3.6)
project(CoreJNI)

add_definitions(-DCORE_LOG_TAG="test")
set(CoreJNI_Root ${CMAKE_SOURCE_DIR}/../../../../src)
include_directories(
    ${CoreJNI_Root}/util/include
    ${CoreJNI_Root}/nativehelper/include
)

set(
    CoreJNI_FILES
    test.cpp
)

add_library(corejni SHARED ${CoreJNI_FILES})
target_link_libraries(
    corejni
    log
)