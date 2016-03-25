cmake_minimum_required(VERSION 2.8.3)
project(laser_projector_bridge)


#==================================================================================================
# options
#==================================================================================================

if(WIN32)
    option(BUILD_JMLASER_PROJECTOR_LIB "Build the jmlaser_projector library" ON)
    option(BUILD_JMLASER_PROJECTOR_TESTS "Build the jmlaser_projector test executables" ON)
    option(BUILD_IMAGE_TRACER "Build the image tracer library" OFF)
else()
    option(BUILD_JMLASER_PROJECTOR_LIB "Build the jmlaser_projector library" OFF)
    option(BUILD_JMLASER_PROJECTOR_TESTS "Build the jmlaser_projector test executables" OFF)
    option(BUILD_IMAGE_TRACER "Build the image tracer library" ON)
endif()

option(BUILD_NETWORK_BRIDGE "Build the Google Protocol Buffers messages" ON)



#==================================================================================================
# packages
#==================================================================================================

if(BUILD_IMAGE_TRACER)
    find_package(OpenCV REQUIRED)
endif()

if(BUILD_NETWORK_BRIDGE)
    find_package(Protobuf REQUIRED)
    find_path(ZMQ_INCLUDE_DIR zmq.h)
    find_library(ZMQ_LIBRARY NAMES zmq)
    set(ZMQ_LIBRARIES ${ZMQ_LIBRARY})
    set( ZMQ_INCLUDE_DIRS ${ZMQ_INCLUDE_DIR})
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(ZMQ DEFAULT_MSG ZMQ_LIBRARY ZMQ_INCLUDE_DIR)
endif()


#==================================================================================================
# flags
#==================================================================================================

if(BUILD_JMLASER_PROJECTOR_LIB)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -std=c++11")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -m32 -Wl,--enable-stdcall-fixup")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32 -Wl,--enable-stdcall-fixup")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32 -Wl,--enable-stdcall-fixup")
endif()



#==================================================================================================
# includes and link directories
#==================================================================================================

include_directories(
    include
    lib
)

if(BUILD_IMAGE_TRACER)
    include_directories(${OpenCV_INCLUDE_DIRS})
    link_directories(${OpenCV_LIBRARY_DIRS})
endif()

if(BUILD_NETWORK_BRIDGE)
    include_directories(${CMAKE_BINARY_DIR})
    include_directories(${PROTOBUF_INCLUDE_DIRS})
    include_directories(${ZMQ_INCLUDE_DIRS})
endif()

include(GNUInstallDirs)



#==================================================================================================
# Google Protocol Buffers Messages
#==================================================================================================

if(BUILD_NETWORK_BRIDGE)
    PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS
        msgs/point.proto
        msgs/shape.proto
        msgs/vector_image.proto
    )
endif()


#==================================================================================================
# libraries
#==================================================================================================

if(BUILD_JMLASER_PROJECTOR_LIB)
    add_library(jmlaser SHARED IMPORTED)
    set_property(TARGET jmlaser PROPERTY IMPORTED_IMPLIB ${PROJECT_SOURCE_DIR}/lib/jmlaser/jmlaser.dll)

    add_library(jmlaser_projector SHARED src/jmlaser_projector.cpp)
    LIST(APPEND ${PROJECT_NAME}-libraries jmlaser_projector)

    add_custom_command(TARGET jmlaser_projector POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/lib/jmlaser/jmlaser.dll"
        ${CMAKE_BINARY_DIR})
endif()


if(BUILD_IMAGE_TRACER)
    add_library(image_tracer SHARED src/image_tracer.cpp)
    LIST(APPEND ${PROJECT_NAME}-libraries image_tracer)
endif()


if(BUILD_NETWORK_BRIDGE)
    add_library(laser_projector_bridge_msgs SHARED ${PROTO_SRCS} ${PROTO_HDRS})
    LIST(APPEND ${PROJECT_NAME}-libraries laser_projector_bridge_msgs)

    add_library(vector_image_subscriber SHARED src/vector_image_subscriber.cpp)
    LIST(APPEND ${PROJECT_NAME}-libraries vector_image_subscriber)
    
    add_library(vector_image_publisher SHARED src/vector_image_publisher.cpp)
    LIST(APPEND ${PROJECT_NAME}-libraries vector_image_publisher)
endif()


#==================================================================================================
# executables
#==================================================================================================

if(BUILD_NETWORK_BRIDGE)
    add_executable(vector_image_subscriber_node src/vector_image_subscriber_node.cpp)
    LIST(APPEND ${PROJECT_NAME}-executables vector_image_subscriber_node)
endif()



#==================================================================================================
# test executables
#==================================================================================================

if(BUILD_JMLASER_PROJECTOR_LIB AND BUILD_JMLASER_PROJECTOR_TESTS)
    add_executable(test_jmlaser_projector lib/jmlaser/test_jmlaser_projector.cpp)
    LIST(APPEND ${PROJECT_NAME}-executables test_jmlaser_projector)
endif()



#==================================================================================================
# libraries link
#==================================================================================================

if(BUILD_JMLASER_PROJECTOR_LIB)
    target_link_libraries(jmlaser_projector jmlaser)
endif()

if(BUILD_IMAGE_TRACER)
    target_link_libraries(image_tracer ${OpenCV_LIBRARIES})
endif()



#==================================================================================================
# executables link
#==================================================================================================

if(BUILD_NETWORK_BRIDGE)
    target_link_libraries(vector_image_subscriber ${ZMQ_LIBRARIES})
    target_link_libraries(vector_image_subscriber_node vector_image_subscriber)
    target_link_libraries(vector_image_publisher ${ZMQ_LIBRARIES})
endif()



#==================================================================================================
# test executables link
#==================================================================================================
if(BUILD_JMLASER_PROJECTOR_LIB AND BUILD_JMLASER_PROJECTOR_TESTS)
    target_link_libraries(test_jmlaser_projector jmlaser_projector)
endif()



#==================================================================================================
# install
#==================================================================================================

if(DEFINED ${PROJECT_NAME}-libraries)
    install(TARGETS ${${PROJECT_NAME}-libraries} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib EXPORT ${PROJECT_NAME}-config)
    install(DIRECTORY include/${PROJECT_NAME}/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})
    install(EXPORT ${PROJECT_NAME}-config DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
endif()

if(BUILD_NETWORK_BRIDGE)
    install(FILES ${PROTO_HDRS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})
endif()


if(DEFINED ${PROJECT_NAME}-executables)
    install(TARGETS ${${PROJECT_NAME}-executables} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME})
endif()
