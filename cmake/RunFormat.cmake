# A standalone version of the git clang-format call used in the project
# The only argument is an optional -DCLANGFORMAT_BASEREF=
get_filename_component(_dir "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)
include("${_dir}/Format.cmake")

if(NOT DEFINED CLANGFORMAT_BASEREF)
	set(CLANGFORMAT_BASEREF "origin/master")
endif()

FormatCommand("git" "." ${CLANGFORMAT_BASEREF} TRUE CMD)
message(STATUS "Invoking: ${CMD}")
execute_process(COMMAND ${CMD})
