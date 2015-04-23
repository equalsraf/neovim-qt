cmake_minimum_required(VERSION 2.8.11)
project(neovim-qt)

if ( NOT EXISTS ${NEOVIM_EXEC} )
	set(NEOVIM_EXEC nvim)
endif ( NOT EXISTS ${NEOVIM_EXEC} )

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR 
		"${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	add_definitions(-Werror -Wall -Wextra -Wno-unused-parameter -std=c++11 -Wfatal-errors)
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# Qt
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Test REQUIRED)

# msgpack
find_package(Msgpack REQUIRED)
include_directories(${MSGPACK_INCLUDE_DIRS})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()

add_subdirectory(src)
add_subdirectory(doc)
enable_testing()
add_subdirectory(test)

# Bindings
find_package(PythonInterp)
if (PYTHONINTERP_FOUND)
	add_custom_target(bindings
		COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/bindings/generate_bindings.py nvim ${CMAKE_SOURCE_DIR}/src/auto
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Generating bindings"
		)
endif()


