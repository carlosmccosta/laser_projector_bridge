cmake_minimum_required(VERSION 2.8.3)
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
# includes and link directories
#==================================================================================================

include_directories(
    include
)



#==================================================================================================
# libraries
#==================================================================================================

add_library(jmlaser SHARED IMPORTED)
set_property(TARGET jmlaser PROPERTY IMPORTED_IMPLIB ${PROJECT_SOURCE_DIR}/lib/jmlaser/jmlaser.dll)

add_library(jmlaser_projector SHARED src/jmlaser_projector.cpp)
LIST(APPEND ${PROJECT_NAME}-libraries jmlaser_projector)

add_library(vector_image_builder SHARED src/vector_image_builder.cpp)
LIST(APPEND ${PROJECT_NAME}-libraries vector_image_builder)

add_custom_command(TARGET jmlaser_projector POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/lib/jmlaser/jmlaser.dll"
    ${CMAKE_BINARY_DIR})



#==================================================================================================
# test executables
#==================================================================================================

add_executable(test_jmlaser_projector src/test_jmlaser_projector.cpp)
LIST(APPEND ${PROJECT_NAME}-executables test_jmlaser_projector)



#==================================================================================================
# libraries link
#==================================================================================================

target_link_libraries(jmlaser_projector jmlaser)



#==================================================================================================
# test executables link
#==================================================================================================

target_link_libraries(test_jmlaser_projector jmlaser_projector vector_image_builder)



#==================================================================================================
# install
#==================================================================================================

if(DEFINED ${PROJECT_NAME}-libraries)
    install(TARGETS ${${PROJECT_NAME}-libraries} DESTINATION lib EXPORT ${PROJECT_NAME}Config)
    install(DIRECTORY include/${PROJECT_NAME}/ DESTINATION include/${PROJECT_NAME})
    install(EXPORT ${PROJECT_NAME}Config DESTINATION share/${PROJECT_NAME}/cmake)
endif()


if(DEFINED ${PROJECT_NAME}-executables)
    install(TARGETS ${${PROJECT_NAME}-executables} DESTINATION lib/${PROJECT_NAME})
endif()
