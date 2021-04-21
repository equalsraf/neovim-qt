#
# CMake wrapper to call windeployqt in Windows
#
# The files are placed in windeployqt folder in the toplevel
# build directory

function(WinDeployQt)
	cmake_parse_arguments(_deploy
		"COMPILER_RUNTIME;FORCE"
		"TARGET"
		"INCLUDE_MODULES;EXCLUDE_MODULES"
		${ARGN}
		)

	if(NOT _deploy_TARGET)
		message(FATAL_ERROR "A TARGET must be specified")
	endif()
	if(CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
		# These flags no longer work in MinGW builds, disable for now
		message(VERBOSE "Disabling windeployqt release/debug options - QTBUG-62103")
	else()
		if(CMAKE_BUILD_TYPE STREQUAL "Debug")
			list(APPEND _ARGS --debug)
		elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
			list(APPEND _ARGS --release-with-debug-info)
		elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
			list(APPEND _ARGS --release)
		endif()
	endif()
	if(_deploy_COMPILER_RUNTIME)
		list(APPEND _ARGS --compiler-runtime)
	endif()
	if(_deploy_FORCE)
		list(APPEND _ARGS --force)
	endif()

	foreach(mod ${_deploy_INCLUDE_MODULES})
		string(TOLOWER ${mod} mod)
		string(REPLACE "qt5::" "" mod ${mod})
		string(REPLACE "qt6::" "" mod ${mod})
		list(APPEND _ARGS "--${mod}")
	endforeach()
	foreach(mod ${_deploy_EXCLUDE_MODULES})
		string(TOLOWER ${mod} mod)
		string(REPLACE "qt5::" "" mod ${mod})
		string(REPLACE "qt6::" "" mod ${mod})
		list(APPEND _ARGS "--no-${mod}")
	endforeach()

	find_program(_deploy_PROGRAM windeployqt
		PATHS $ENV{QTDIR}/bin/)
	if(_deploy_PROGRAM)
		message(STATUS "Found ${_deploy_PROGRAM}")
	else()
		message(FATAL_ERROR "Unable to find windeployqt")
	endif()

	if(COMPILER_RUNTIME AND NOT $ENV{VVVV})
		message(STATUS "not set, the VC++ redistributable installer will NOT be bundled")
	endif()

	add_custom_target(windeployqt ALL ${_deploy_PROGRAM} --dir ${PROJECT_BINARY_DIR}/windeployqt ${_ARGS}
		$<TARGET_FILE:${_deploy_TARGET}>
		DEPENDS ${_deploy_TARGET}
		COMMENT "Preparing Qt runtime dependencies")
endfunction()


