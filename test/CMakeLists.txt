
include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(tst_connector tst_connector.cpp)
target_link_libraries(tst_connector Qt5::Network Qt5::Test ${MSGPACK_LIBRARIES} neovim-qt)
