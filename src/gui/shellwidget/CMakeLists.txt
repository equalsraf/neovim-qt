cmake_minimum_required(VERSION 2.8.12)
if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)
project(qshellwidget)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Qt
set(CMAKE_AUTOMOC ON)
find_package(${TARGET_QT}Widgets REQUIRED)
find_package(${TARGET_QT}Test REQUIRED)

if (WIN32 AND USE_STATIC_QT)
	add_definitions(-DUSE_STATIC_QT)
endif ()

set(SOURCES
  cell.cpp
  cursor.cpp
  highlight.cpp
  helpers.cpp
  konsole_wcwidth.cpp
  shellcontents.cpp
  shellwidget.cpp)

add_library(qshellwidget STATIC ${SOURCES})
target_link_libraries(qshellwidget ${TARGET_QT}::Widgets)

add_executable(example EXCLUDE_FROM_ALL example.cpp)
target_link_libraries(example qshellwidget )
add_executable(example2 EXCLUDE_FROM_ALL example2.cpp)
target_link_libraries(example2 qshellwidget )
add_executable(example3 EXCLUDE_FROM_ALL example3.cpp)
target_link_libraries(example3 qshellwidget )

add_executable(fontinfo EXCLUDE_FROM_ALL fontinfo.cpp)
target_link_libraries(fontinfo ${TARGET_QT}::Widgets )
