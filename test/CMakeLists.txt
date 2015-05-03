
include_directories(${CMAKE_SOURCE_DIR}/src)

function(add_xtest SOURCE_NAME)
	add_executable(${SOURCE_NAME} ${SOURCE_NAME}.cpp)
	target_link_libraries(${SOURCE_NAME} Qt5::Network Qt5::Test ${MSGPACK_LIBRARIES} neovim-qt)
	add_test(NAME ${SOURCE_NAME} COMMAND ${SOURCE_NAME})
endfunction()

add_xtest(tst_neovimobject)
add_xtest(tst_neovimconnector)
add_xtest(tst_callallmethods)
add_xtest(tst_encoding)
add_xtest(tst_msgpackiodevice)
