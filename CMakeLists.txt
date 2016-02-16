cmake_minimum_required(VERSION 3.4)
project(laser_projector_bridge)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -std=c++11 -Wall")

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32 -Wl,--enable-stdcall-fixup")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32 -Wl,--enable-stdcall-fixup")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32 -Wl,--enable-stdcall-fixup")

include_directories(
    include
    lib
)

add_library(jmlaser SHARED IMPORTED)
set_property(TARGET jmlaser PROPERTY IMPORTED_IMPLIB ${PROJECT_SOURCE_DIR}/lib/jmlaser/jmlaser.dll)

set(SOURCE_FILES lib/jmlaser/test_jmlaser_api.cpp)
add_executable(laser_projector_bridge ${SOURCE_FILES})

add_custom_command(TARGET laser_projector_bridge POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/lib/jmlaser/jmlaser.dll"
        ${CMAKE_BINARY_DIR})

target_link_libraries(laser_projector_bridge jmlaser)
