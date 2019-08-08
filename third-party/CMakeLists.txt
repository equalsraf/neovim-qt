# This is a minimal CMake project to fetch and build third party
# dependencies
cmake_minimum_required(VERSION 2.8.11)
if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)
project(neovim-qt-deps)

#
# Get Msgpack
#
set(MSGPACK_VERSION 3.2.0)
set(MSGPACK_URL https://github.com/msgpack/msgpack-c/archive/cpp-${MSGPACK_VERSION}.tar.gz)
set(MSGPACK_SHA256 ff865a36bad5c72b8e7ebc4b7cf5f27a820fce4faff9c571c1791e3728355a39)

message(STATUS "Downloading Msgpack...")
set(MSGPACK_TARBALL msgpack-${MSGPACK_VERSION}.tar.gz)
file(DOWNLOAD ${MSGPACK_URL} ${CMAKE_CURRENT_SOURCE_DIR}/${MSGPACK_TARBALL}
	INACTIVITY_TIMEOUT 30
	EXPECTED_HASH SHA256=${MSGPACK_SHA256})
execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz ${MSGPACK_TARBALL}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	RESULT_VARIABLE rv)
if(NOT rv EQUAL 0)
  message(FATAL_ERROR "Failed to extract ${MSGPACK_TARBALL}")
endif()

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(MSGPACK_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/msgpack-c-cpp-${MSGPACK_VERSION}/)
add_subdirectory(${MSGPACK_SOURCE_DIR} EXCLUDE_FROM_ALL)

# Similar enough to FindMsgpack
set(MSGPACK_INCLUDE_DIRS ${MSGPACK_SOURCE_DIR}/include PARENT_SCOPE)
set(MSGPACK_LIBRARIES msgpackc-static PARENT_SCOPE)
