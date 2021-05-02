cmake_minimum_required(VERSION 2.8.12)
if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

# Neovim-Qt Version, used by --version update before release
# 9999 = Development Pre-Release
project(neovim-qt VERSION 0.2.17.9999)

INCLUDE(CPack)

if(NOT EXISTS ${NEOVIM_EXEC})
	set(NEOVIM_EXEC nvim)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
		"${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wunused-variable")

	if(ENABLE_CLAZY)
		string(CONCAT CLAZY_CHECKS
			"level0",
			"no-fully-qualified-moc-types,"
			"no-lambda-in-connect,"
			"no-overloaded-signal,"
			"no-qenums,"
			"level1",
			"no-connect-3arg-lambda,"
			"no-inefficient-qlist-soft,"
			"no-overridden-signal,"
			"no-qproperty-without-notify,"
			"no-range-loop,"
			)

		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xclang -plugin-arg-clazy -Xclang ${CLAZY_CHECKS} -Wno-deprecated-declarations -Werror")
	endif()
endif()

option(USE_GCOV "Enable gcov support" OFF)
if(USE_GCOV)
	message(STATUS "Enabling coverage")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# to generate a compile_commands.json usable by ycm and other tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# konsole_wcwidth.h
include_directories(third-party)

# homebrew/macrpots/fink etc.
include(MacOSXPaths)

# Qt
set(CMAKE_AUTOMOC ON)

# Build against Qt5 or Qt6
if(NOT DEFINED QT_PACKAGES)
	set(QT_PACKAGES Qt5 Qt6)
else()
	message(WARNING "Overriding Qt package names: ${QT_PACKAGES}")
endif()
set(REQ_QT_MODULES Core Network Test Widgets)
find_package(QT NAMES ${QT_PACKAGES} COMPONENTS ${REQ_QT_MODULES} REQUIRED)
set(TARGET_QT "Qt${QT_VERSION_MAJOR}")
if(${QT_VERSION_MAJOR} VERSION_GREATER_EQUAL 6.0)
	# Qt6 uses SvgWidgets instead of Svg, and requires Core5Compat
	set(REQ_QT_MODULES ${REQ_QT_MODULES} SvgWidgets Core5Compat)
	set(CMAKE_CXX_STANDARD 17)
else()
	set(REQ_QT_MODULES ${REQ_QT_MODULES} Svg)
endif()
message(STATUS "Building against Qt${QT_VERSION_MAJOR}.${QT_VERSION_MINOR} ${REQ_QT_MODULES}")
find_package(${TARGET_QT} COMPONENTS ${REQ_QT_MODULES} REQUIRED)

if(Qt${QT_VERSION_MAJOR}Core_VERSION VERSION_LESS 5.8)
	message(FATAL_ERROR "Minimum supported Qt5 version is 5.8!")
endif()

# msgpack
option(USE_SYSTEM_MSGPACK "Use system msgpack libraries " OFF)
if(USE_SYSTEM_MSGPACK)
	find_package(Msgpack REQUIRED)
else()
	add_subdirectory(third-party)
endif()
include_directories(${MSGPACK_INCLUDE_DIRS})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()

# Place targets in bin/ or lib/ for all build configurations
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
foreach(CFGNAME ${CMAKE_CONFIGURATION_TYPES})
  string(TOUPPER ${CFGNAME} CFGNAME)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CFGNAME} ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CFGNAME} ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CFGNAME} ${CMAKE_BINARY_DIR}/lib)
endforeach()

if(MSVC)
  # Allow use of deprecated function names in MSVC (read/write)
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

add_subdirectory(src)
add_subdirectory(doc)

option(ENABLE_TESTS "Build tests" ON)
if(ENABLE_TESTS)
	enable_testing()
	add_subdirectory(test)
	add_subdirectory(src/gui/shellwidget/test)
endif()

# Bindings
find_package(PythonInterp)
if (PYTHONINTERP_FOUND)
	set(NVIM "nvim" CACHE STRING "Path to nvim executable")
	add_custom_target(bindings
		COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/bindings/generate_bindings.py ${NVIM} ${CMAKE_SOURCE_DIR}/src/auto
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Generating bindings"
		)

	add_custom_target(bindings-preview
		COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/bindings/generate_bindings.py ${NVIM}
		)
endif()


