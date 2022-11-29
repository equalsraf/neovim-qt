cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)

project(qshellwidget)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Qt
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
if(ENABLE_TESTS)
    find_package(Qt5 REQUIRED COMPONENTS Test)
endif()

if (WIN32 AND USE_STATIC_QT)
    add_compile_definitions(-DUSE_STATIC_QT)
endif ()

add_library(qshellwidget STATIC)

target_sources(qshellwidget
  PRIVATE
    cell.cpp
    cell.h
    cursor.cpp
    cursor.h
    helpers.cpp
    helpers.h
    highlight.cpp
    highlight.h
    konsole_wcwidth.cpp
    konsole_wcwidth.h
    shellcontents.cpp
    shellcontents.h
    shellwidget.cpp
    shellwidget.h
)

target_link_libraries(qshellwidget PUBLIC Qt5::Widgets)

add_executable(example EXCLUDE_FROM_ALL example.cpp)
target_link_libraries(example PRIVATE qshellwidget)

add_executable(example2 EXCLUDE_FROM_ALL example2.cpp)
target_link_libraries(example2 PRIVATE qshellwidget)

add_executable(example3 EXCLUDE_FROM_ALL example3.cpp)
target_link_libraries(example3 PRIVATE qshellwidget)

add_executable(fontinfo EXCLUDE_FROM_ALL fontinfo.cpp)
target_link_libraries(fontinfo PRIVATE Qt5::Widgets)
