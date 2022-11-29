configure_file(version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(neovim-qt STATIC)

target_sources(neovim-qt
  PRIVATE
    function.cpp
    function.h
    msgpackiodevice.cpp
    msgpackiodevice.h
    msgpackrequest.cpp
    msgpackrequest.h
    neovimconnector.cpp
    neovimconnector.h
    neovimconnectorhelper.cpp
    neovimconnectorhelper.h
    util.cpp
    util.h
)

if(WIN32)
    target_sources(neovim-qt
      PRIVATE
        stdinreader.cpp
        stdinreader.h
    )
endif()

target_link_libraries(neovim-qt PUBLIC Qt5::Network msgpackc-static)

add_subdirectory(auto)
add_subdirectory(gui)
