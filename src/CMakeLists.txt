
configure_file(version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(${QT_VERSION_MAJOR} VERSION_GREATER_EQUAL 6.0)
	include_directories(${Qt6SvgWidgets_INCLUDE_DIRS})
	add_definitions(${Qt6SvgWidgets_DEFINITIONS})
	include_directories(${Qt6Core5Compat_INCLUDE_DIRS})
	add_definitions(${Qt6Core5Compat_DEFINITIONS})
endif()

set(NEOVIM_QT_SOURCES
  auto/neovimapi0.cpp
  auto/neovimapi1.cpp
  auto/neovimapi2.cpp
  auto/neovimapi3.cpp
  auto/neovimapi4.cpp
  auto/neovimapi5.cpp
  auto/neovimapi6.cpp
  function.cpp
  msgpackiodevice.cpp
  msgpackrequest.cpp
  neovimconnector.cpp
  neovimconnectorhelper.cpp
  util.cpp)

if(WIN32)
  list(APPEND NEOVIM_QT_SOURCES stdinreader.cpp)
endif()

add_library(neovim-qt STATIC ${NEOVIM_QT_SOURCES})
target_link_libraries(neovim-qt ${TARGET_QT}::Network ${MSGPACK_LIBRARIES})
if(${QT_VERSION_MAJOR} VERSION_GREATER_EQUAL 6.0)
	target_link_libraries(neovim-qt Qt6::Core5Compat)
endif()

add_subdirectory(gui)
