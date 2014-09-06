
include_directories(${CMAKE_SOURCE_DIR}/src)

function(add_xtest EXEC_NAME)
	add_test(${EXEC_NAME} ${EXEC_NAME})
endfunction()

add_executable(tst_connector tst_connector.cpp)
target_link_libraries(tst_connector Qt5::Network Qt5::Test ${MSGPACK_LIBRARIES} neovim-qt)
add_xtest(tst_connector)

add_executable(tst_neovimobject tst_neovimobject.cpp)
target_link_libraries(tst_neovimobject Qt5::Network Qt5::Test ${MSGPACK_LIBRARIES} neovim-qt)
add_xtest(tst_neovimobject)

add_executable(tst_callallmethods tst_callallmethods.cpp)
target_link_libraries(tst_callallmethods Qt5::Network Qt5::Test ${MSGPACK_LIBRARIES} neovim-qt)
add_xtest(tst_callallmethods)
