
set(NEOVIM_QT_SOURCES util.cpp neovimconnector.cpp neovimconnectorhelper.cpp function.cpp msgpackrequest.cpp msgpackiodevice.cpp auto/1/neovim.cpp auto/2/neovim.cpp)
if(WIN32)
  list(APPEND NEOVIM_QT_SOURCES stdinreader.cpp)
endif()

add_library(neovim-qt STATIC ${NEOVIM_QT_SOURCES})
target_link_libraries(neovim-qt Qt5::Network ${MSGPACK_LIBRARIES})

add_subdirectory(gui)
