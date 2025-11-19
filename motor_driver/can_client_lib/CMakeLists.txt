cmake_minimum_required(VERSION 3.18.4)

project(motordriver_can_lib)
set(PROJECT_VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)


# add dependencies
include(cmake/CPM.cmake)
include(ExternalProject)
include(CheckIncludeFile)

#CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.8.0")
CPMAddPackage("gh:Neargye/magic_enum@0.8.2")
CPMAddPackage("gh:fmtlib/fmt#9.1.0")
CPMAddPackage("gh:bfgroup/Lyra#1.6.1")
CPMAddPackage(
        NAME socketcan_cpp
        GIT_TAG "master"  # this repo does not has git tags
        GITHUB_REPOSITORY siposcsaba89/socketcan-cpp
)

CPMAddPackage("gl:eidheim/Simple-WebSocket-Server@2.0.2")

## add SOT Can lib
CPMAddPackage("gl:JeyRunner/can-sot-protocol#c2b1a40c")
# gen header files from can sot protocol spec for master (this lib) and client (in ../firmware)
add_custom_target(
        genCanSotHeaders
        COMMAND bash -c "./createCanSotHeaders.sh ${can-sot-protocol_BINARY_DIR}/" # & pwd & echo 'dir: ${can-sot-protocol_BINARY_DIR}'"
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        DEPENDS canSotCli
)

## using the old manually implemented can communication protocol
# include(src/leacy/CMakeLists.txt)
#[[
check_include_file("linux/can.h" HAVE_LINUX_CAN_H)
if (HAVE_LINUX_CAN_H)
    message(STATUS "Found linux can headers, enabling.")
    add_compile_definitions(-DHAVE_SOCKETCAN_HEADERS=1)
else()
    message(STATUS "Linux can headers not found, disabling")
endif()
#add_compile_definitions(-DHAVE_SOCKETCAN_HEADERS=1)
]]

#[[
ExternalProject_Add(libcan_external
        GIT_REPOSITORY https://github.com/matthiasbock/libcan.git
        #GIT_TAG v0.14.8
        #PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/uWebSockets"

        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ${MAKE}
        INSTALL_COMMAND ""
        #INSTALL_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/uWebSockets/src/uWebSockets_external/libuWS.${LIB_SUFIX} ${OUTPUT_LIB}
        )
set(libcan_external_LIB_DIR "${CMAKE_BINARY_DIR}/libcan_external-prefix/src/libcan_external/")
set(libcan_external_LIB libcan.so)
set(libcan_external_INCLUDE_DIR "${CMAKE_BINARY_DIR}/libcan_external-prefix/src/libcan_external/include")
]]


## lib
file(GLOB src_files src/can_master_lib/**)
add_library(${PROJECT_NAME} ${src_files})

target_link_directories(${PROJECT_NAME}
        PUBLIC
        #${libcan_external_LIB_DIR}
        )
#add_dependencies(${PROJECT_NAME} libcan_external)
target_link_libraries(${PROJECT_NAME}
        PUBLIC
        #${libcan_external_LIB}
        CanSotProtocol-baseLib
        magic_enum
        fmt::fmt
        )


## package lib
#[[
packageProject(
        NAME ${PROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        NAMESPACE ${PROJECT_NAME}
        BINARY_DIR ${PROJECT_BINARY_DIR}
        INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
        INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
        #VERSION_HEADER "${VERSION_HEADER_LOCATION}"
        COMPATIBILITY SameMajorVersion
        DEPENDENCIES ""
)
]]



#######################################
## example app
file(GLOB app_src_files src/example_app/** src/example_app/**/**)
add_executable(example_app ${app_src_files})
message(STATUS "example_app src      ${app_src_files}")

#add_dependencies(can_client_lib libcan_external)
target_link_libraries(example_app ${PROJECT_NAME} fmt::fmt lyra simple-websocket-server)
target_include_directories(example_app PUBLIC src/can_master_lib)



#######################################
## configuration tool app
file(GLOB configuration_app_src_files src/configuration_app/** app_src_files src/configuration_app/**/**)
add_executable(configuration_tool ${configuration_app_src_files})
message(STATUS "src      ${configuration_app_src_files}")

#add_dependencies(can_client_lib libcan_external)
target_link_libraries(configuration_tool ${PROJECT_NAME} fmt::fmt lyra simple-websocket-server)
target_include_directories(configuration_tool PUBLIC src/can_master_lib)
