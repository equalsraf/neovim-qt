set(NEOVIM_QT_SOURCES neovimconnector.cpp function.cpp auto/neovim.cpp util.cpp)
add_definitions(-DQT_NO_CAST_FROM_ASCII)

add_library(neovim-qt ${NEOVIM_QT_SOURCES})
target_link_libraries(neovim-qt Qt5::Network ${MSGPACK_LIBRARIES})


