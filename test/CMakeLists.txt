
include_directories(${CMAKE_SOURCE_DIR}/src)
if (WIN32 AND USE_STATIC_QT)
	add_definitions(-DUSE_STATIC_QT)
endif ()

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
function(add_xtest SOURCE_NAME)
	add_executable(${SOURCE_NAME} ${SOURCE_NAME}.cpp ${ARGV1} ${ARGV2})
	target_link_libraries(${SOURCE_NAME} Qt5::Network Qt5::Test ${MSGPACK_LIBRARIES} neovim-qt)
	add_test(NAME ${SOURCE_NAME} COMMAND ${SOURCE_NAME})
	add_dependencies(check ${SOURCE_NAME})
endfunction()

function(add_xtest_gui SOURCE_NAME)
	add_executable(${SOURCE_NAME} ${SOURCE_NAME}.cpp)
	target_link_libraries(${SOURCE_NAME} Qt5::Network Qt5::Test ${MSGPACK_LIBRARIES} neovim-qt Qt5::Widgets neovim-qt-gui)
	add_test(NAME ${SOURCE_NAME} COMMAND ${SOURCE_NAME}
		# Run GUI tests from source dir, they depend on src files
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
	add_dependencies(check ${SOURCE_NAME})
endfunction()

add_xtest(tst_neovimobject)
add_xtest(tst_neovimconnector)
add_xtest(tst_callallmethods)
add_xtest(tst_encoding)
add_xtest(tst_msgpackiodevice)
add_xtest(tst_input ${CMAKE_SOURCE_DIR}/src/gui/input.cpp)
add_xtest_gui(tst_shell)
