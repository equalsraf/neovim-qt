
include_directories(${CMAKE_SOURCE_DIR}/src)

function(add_xtest EXEC_NAME)
	add_executable(${EXEC_NAME} ${EXEC_NAME}.cpp)
	target_link_libraries(${EXEC_NAME} Qt5::Network Qt5::Test ${MSGPACK_LIBRARIES} neovim-qt)
	add_test(${EXEC_NAME} ${EXEC_NAME})
endfunction()

add_xtest(tst_connector)
add_xtest(tst_neovimobject)
add_xtest(tst_callallmethods)
add_xtest(tst_encoding)
