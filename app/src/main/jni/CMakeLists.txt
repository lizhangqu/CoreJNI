cmake_minimum_required(VERSION 3.6)
project(CoreJNI)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fexceptions")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE} -s")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -s")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O1 -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1 -g")

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