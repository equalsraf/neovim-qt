cmake_minimum_required(VERSION 2.8.11)
project(qshellwidget)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Qt
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

set(SOURCES shellcontents.cpp helpers.cpp shellwidget.cpp konsole_wcwidth.cpp)
add_library(qshellwidget STATIC ${SOURCES})
target_link_libraries(qshellwidget Qt5::Widgets)

add_executable(example example.cpp)
target_link_libraries(example qshellwidget )
add_executable(example2 example2.cpp)
target_link_libraries(example2 qshellwidget )
add_executable(example3 example3.cpp)
target_link_libraries(example3 qshellwidget )

add_executable(fontinfo fontinfo.cpp)
target_link_libraries(fontinfo Qt5::Widgets )

enable_testing()
add_subdirectory(test)
