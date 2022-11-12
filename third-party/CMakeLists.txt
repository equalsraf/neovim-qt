# This is a minimal CMake project to fetch and build third party
# dependencies
cmake_minimum_required(VERSION 3.16)

cmake_policy(SET CMP0048 NEW)
if (POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

project(neovim-qt-deps)

include(FetchContent)

set(MSGPACK_VERSION 3.2.0)
FetchContent_Declare(
    msgpackc
    URL "https://github.com/msgpack/msgpack-c/archive/cpp-${MSGPACK_VERSION}.tar.gz"
    URL_HASH SHA256=ff865a36bad5c72b8e7ebc4b7cf5f27a820fce4faff9c571c1791e3728355a39
)
FetchContent_MakeAvailable(msgpackc)
