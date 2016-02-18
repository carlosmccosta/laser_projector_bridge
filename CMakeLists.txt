cmake_minimum_required(VERSION 3.4)
project(laser_projector_bridge)



#==================================================================================================
# flags
#==================================================================================================

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -std=c++11")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -m32 -Wl,--enable-stdcall-fixup")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32 -Wl,--enable-stdcall-fixup")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32 -Wl,--enable-stdcall-fixup")



#==================================================================================================
# includes
#==================================================================================================

include_directories(
    include
    lib
)



#==================================================================================================
# libraries
#==================================================================================================

add_library(jmlaser SHARED IMPORTED)
set_property(TARGET jmlaser PROPERTY IMPORTED_IMPLIB ${PROJECT_SOURCE_DIR}/lib/jmlaser/jmlaser.dll)

add_library(jmlaser_projector SHARED src/JMLaserProjector.cpp)
add_custom_command(TARGET jmlaser_projector POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/lib/jmlaser/jmlaser.dll"
        ${CMAKE_BINARY_DIR})



#==================================================================================================
# executables
#==================================================================================================



#==================================================================================================
# test executables
#==================================================================================================

add_executable(test_jmlaser_projector lib/jmlaser/test_jmlaser_projector.cpp)



#==================================================================================================
# libraries link
#==================================================================================================

target_link_libraries(jmlaser_projector jmlaser)



#==================================================================================================
# executables link
#==================================================================================================



#==================================================================================================
# test executables link
#==================================================================================================

target_link_libraries(test_jmlaser_projector jmlaser_projector)
