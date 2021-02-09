project(qcommandline)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)


if (WIN32 AND USE_STATIC_QT)
	add_definitions(-DUSE_STATIC_QT)
endif ()

set(SOURCES
  blockdisplay.cpp
  extcmdlinewidget.cpp
  linemodel.cpp
  position.cpp
  )

add_library(extcmdline STATIC ${SOURCES})
target_link_libraries(extcmdline Qt5::Widgets Qt5::Network)
target_include_directories(extcmdline PUBLIC ../shellwidget)

