
add_library(compat STATIC compat_qt${QT_VERSION_MAJOR}.cpp)
target_link_libraries(compat Qt${QT_VERSION_MAJOR}::Core)
