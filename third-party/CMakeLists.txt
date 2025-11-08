include(FetchContent)

# Set the timestamps of extracted contents to the time of the extraction,
# instead of the timestamps in the tarball, if cmake is new enough
if(CMAKE_VERSION VERSION_GREATER "3.24")
    cmake_policy(SET CMP0135 NEW)
endif()

#
# Get Msgpack
#

set(MSGPACK_VERSION 6.1.0)
set(MSGPACK_SHA256 674119f1a85b5f2ecc4c7d5c2859edf50c0b05e0c10aa0df85eefa2c8c14b796)

set(BUILD_SHARED_LIBS OFF)
set(MSGPACK_BUILD_EXAMPLES OFF)

FetchContent_Declare(
    msgpack-c
    URL https://github.com/msgpack/msgpack-c/releases/download/c-${MSGPACK_VERSION}/msgpack-c-${MSGPACK_VERSION}.tar.gz
    URL_HASH SHA256=${MSGPACK_SHA256}
)
FetchContent_MakeAvailable(msgpack-c)
