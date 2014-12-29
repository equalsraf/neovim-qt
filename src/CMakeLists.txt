
set(NEOVIM_QT_SOURCES util.cpp neovimconnector.cpp function.cpp neovimrequest.cpp auto/neovim.cpp)
add_library(neovim-qt ${NEOVIM_QT_SOURCES})
target_link_libraries(neovim-qt Qt5::Network ${MSGPACK_LIBRARIES})

qt5_add_resources(NEOVIM_RCC_SOURCES images.qrc)
add_executable(nvim-qt main.cpp shell.cpp input.cpp ${NEOVIM_RCC_SOURCES})
target_link_libraries(nvim-qt Qt5::Network Qt5::Widgets ${MSGPACK_LIBRARIES} neovim-qt)

