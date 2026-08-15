function(FormatCommand GITPRG PROJECTDIR BASEREF DIFF OUTVAR)
	set(COMMON_ARGS --style file:${PROJECTDIR}/contrib/clang-format.txt ${BASEREF})

	set(FLAGS)
	if(DIFF)
		list(APPEND FLAGS --diff)
	endif()

	set(${OUTVAR} ${GITPRG} clang-format ${FLAGS} ${COMMON_ARGS} PARENT_SCOPE)
endfunction()


function(ClangFormatTargets BASEREF)
	find_program(GIT_PROGRAM git)
	find_program(CLANGFORMAT_PROGRAM clang-format)

	if(GIT_PROGRAM AND CLANGFORMAT_PROGRAM)
		execute_process(
			COMMAND ${GIT_PROGRAM} clang-format -h
			WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
			RESULT_VARIABLE GITHELPER_STATUS
			OUTPUT_QUIET)
	endif()

	set(COMMON_ARGS --style file:${PROJECT_SOURCE_DIR}/contrib/clang-format.txt ${BASEREF})

	if(GITHELPER_STATUS EQUAL 0)
		message(STATUS "Enabling format target against ${BASEREF}")


		FormatCommand(${GIT_PROGRAM} ${PROJECT_SOURCE_DIR} ${BASEREF} FALSE CMD)
		message(DEBUG "format cmd: ${CMD}")
		FormatCommand(${GIT_PROGRAM} ${PROJECT_SOURCE_DIR} ${BASEREF} TRUE DIFFCMD)
		message(DEBUG "format diff cmd: ${DIFFCMD}")

		add_custom_target(format
			WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
			COMMAND ${CMD})
		add_custom_target(formatdiff
			WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
			COMMAND ${DIFFCMD})
	else()
		message(STATUS "format: Failed to run git clang-format: ${GITHELPER_STATUS}")
		set(COMMAND_ARGS
			# show error message & fail
			COMMAND ${CMAKE_COMMAND} -E echo
			"format target requires git/clang-format with a working git clang-format helper"
			COMMAND ${CMAKE_COMMAND} -E false)

		add_custom_target(format ${COMMAND_ARGS})
		add_custom_target(formatdiff ${COMMAND_ARGS})
	endif()
endfunction()
