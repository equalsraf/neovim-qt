
include_directories(..)
qt5_add_resources(NEOVIM_RCC_SOURCES images.qrc)
add_executable(nvim-qt main.cpp shell.cpp input.cpp ${NEOVIM_RCC_SOURCES})
target_link_libraries(nvim-qt Qt5::Network Qt5::Widgets ${MSGPACK_LIBRARIES} neovim-qt)

