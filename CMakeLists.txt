cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0069 NEW)

# Neovim-Qt Version, used by --version update before release
# 9999 = Development Pre-Release
project(neovim-qt VERSION 0.2.18.9999)

if(NOT EXISTS ${NEOVIM_EXEC})
	set(NEOVIM_EXEC nvim)
endif()

option(USE_GCOV "Enable gcov support" OFF)
option(ENABLE_CLAZY "Build with KDE Clang Clazy Linter" OFF)
option(ENABLE_TIDY "Build with Clang Tidy Linter" OFF)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
		"${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wunused-variable -std=c++11")

	# Qt Static Analysis - Clang-Tidy based extension
	if(ENABLE_CLAZY)
		string(CONCAT CLAZY_CHECKS
			"level0,"
			"level1,"
			"no-inefficient-qlist-soft,"
			)

		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Xclang -plugin-arg-clazy -Xclang ${CLAZY_CHECKS} -Wno-deprecated-declarations -Werror")
	endif()

	# Clang-Tidy Static Analysis
	if(ENABLE_TIDY)
		string(CONCAT TIDY_CHECKS
			"-*,"
			"bugprone-*,"
			"-bugprone-branch-clone,"
			"-bugprone-inaccurate-erase,"
			"-bugprone-narrowing-conversions,"
			"-bugprone-parent-virtual-call,"
			"-bugprone-suspicious-include,"
			"-bugprone-undefined-memory-manipulation,"
			"cppcoreguidelines-*,"
			"-cppcoreguidelines-avoid-c-arrays,"
			"-cppcoreguidelines-avoid-goto,"
			"-cppcoreguidelines-avoid-magic-numbers,"
			"-cppcoreguidelines-avoid-non-const-global-variables,"
			"-cppcoreguidelines-explicit-virtual-functions,"
			"-cppcoreguidelines-init-variables,"
			"-cppcoreguidelines-macro-usage,"
			"-cppcoreguidelines-narrowing-conversions,"
			"-cppcoreguidelines-non-private-member-variables-in-classes,"
			"-cppcoreguidelines-owning-memory,"
			"-cppcoreguidelines-pro-bounds-array-to-pointer-decay,"
			"-cppcoreguidelines-pro-bounds-pointer-arithmetic,"
			"-cppcoreguidelines-pro-type-cstyle-cast,"
			"-cppcoreguidelines-pro-type-member-init,"
			"-cppcoreguidelines-pro-type-static-cast-downcast,"
			"-cppcoreguidelines-pro-type-vararg,"
			"-cppcoreguidelines-special-member-functions,"
			"modernize-*,"
			"-modernize-avoid-c-arrays,"
			"-modernize-loop-convert,"
			"-modernize-pass-by-value,"
			"-modernize-raw-string-literal,"
			"-modernize-return-braced-init-list,"
			"-modernize-use-auto,"
			"-modernize-use-nullptr,"
			"-modernize-use-override,"
			"-modernize-use-trailing-return-type,"
			"-modernize-use-using,"
			"performance-*,"
			"-performance-no-automatic-move,"
			"-performance-unnecessary-copy-initialization,"
			"-performance-unnecessary-value-param,"
			"readability-*,"
			"-readability-braces-around-statements,"
			"-readability-braces-around-statements,"
			"-readability-container-size-empty,"
			"-readability-convert-member-functions-to-static,"
			"-readability-delete-null-pointer,"
			"-readability-else-after-return,"
			"-readability-function-cognitive-complexity,"
			"-readability-function-size,"
			"-readability-identifier-naming,"
			"-readability-implicit-bool-cast,"
			"-readability-implicit-bool-conversion,"
			"-readability-inconsistent-declaration-parameter-name,"
			"-readability-isolate-declaration,"
			"-readability-magic-numbers,"
			"-readability-make-member-function-const,"
			"-readability-named-parameter,"
			"-readability-qualified-auto,"
			"-readability-redundant-access-specifiers,"
			"-readability-redundant-control-flow,"
			"-readability-redundant-member-init,"
			"-readability-simplify-boolean-expr,"
			"-readability-static-accessed-through-instance,"
			"-readability-uppercase-literal-suffix,"
			)
		if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
			message("ENABLE_TIDY requires CMake >= 3.6.0")
		else()
			set(CMAKE_CXX_CLANG_TIDY clang-tidy "-checks=${TIDY_CHECKS};-warnings-as-errors=*")
		endif()
	endif()

	# Code Coverage Report
	if(USE_GCOV)
		message(STATUS "Enabling coverage")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
	endif()

endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

# to generate a compile_commands.json usable by ycm and other tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# konsole_wcwidth.h
include_directories(third-party)

# homebrew/macrpots/fink etc.
include(MacOSXPaths)

# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 5.8 REQUIRED COMPONENTS Core Network Svg Widgets)

# msgpack
option(USE_SYSTEM_MSGPACK "Use system msgpack libraries " OFF)
if(USE_SYSTEM_MSGPACK)
	find_package(Msgpack REQUIRED)
else()
	add_subdirectory(third-party)
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(-DQT_NO_DEBUG_OUTPUT)
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
  add_compile_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

add_subdirectory(src)
add_subdirectory(doc)

option(ENABLE_TESTS "Build tests" OFF)
if(ENABLE_TESTS)
    find_package(Qt5 REQUIRED COMPONENTS Test)
	enable_testing()
	add_subdirectory(test)
	add_subdirectory(src/gui/shellwidget/test)
endif()

# Bindings
find_package(PythonInterp)
if (PYTHONINTERP_FOUND)
	set(NVIM "nvim" CACHE STRING "Path to nvim executable")
	add_custom_target(bindings
        COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/bindings/generate_bindings.py
          ${NVIM} ${PROJECT_SOURCE_DIR}/src/auto
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		COMMENT "Generating bindings"
    )

	add_custom_target(bindings-preview
        COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/bindings/generate_bindings.py ${NVIM}
    )
endif()

include(CPackOptions)
