
include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(example1 example1.cpp)
target_link_libraries(example1 Qt5::Network ${MSGPACK_LIBRARIES} neovim-qt)

add_executable(example2 example2.cpp)
target_link_libraries(example2 Qt5::Widgets Qt5::Network ${MSGPACK_LIBRARIES} neovim-qt)

add_executable(example3 example3.cpp)
target_link_libraries(example3 Qt5::Network ${MSGPACK_LIBRARIES} neovim-qt)
