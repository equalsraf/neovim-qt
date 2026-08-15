# A standalone version of the git clang-format call used in the project
# Arguments: -DCLANGFORMAT_BASEREF= -DCLANGFORMAT_OUTFILE
get_filename_component(_dir "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)
include("${_dir}/Format.cmake")

# Default arguments
if(NOT DEFINED)
	set(CLANGFORMAT_OUTFILE "clang_format.patch")
endif()
if(NOT DEFINED CLANGFORMAT_BASEREF)
	set(CLANGFORMAT_BASEREF "origin/master")
endif()

FormatCommand("git" "." ${CLANGFORMAT_BASEREF} TRUE CMD)
message(STATUS "Invoking: ${CMD}")
execute_process(COMMAND ${CMD} RESULT_VARIABLE STATUS OUTPUT_VARIABLE OUT ERROR_VARIABLE ERR OUTPUT_STRIP_TRAILING_WHITESPACE)

if(STATUS EQUAL 0)
	message(STATUS "Format exited with status 0")
else()
	string(LENGTH "${ERR}" ERRLEN)
	string(LENGTH "${OUT}" OUTLEN)

	# git clang-format-diff returns non 0 when changes were made
	# assume clean stderr means patch on stdout
	if (ERRLEN EQUAL 0)
		message(STATUS "Format exited with output, writing ${CLANGFORMAT_OUTFILE}")
		file(WRITE ${CLANGFORMAT_OUTFILE} ${OUT})
	else()
		message(SEND_ERROR ${ERR})
		message(FATAL_ERROR "git clang-format command failed: ${STATUS} (err: ${ERRLEN}, out: ${OUTLEN})")
	endif()
endif()

