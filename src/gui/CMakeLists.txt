set(QTLIBS Qt5::Network Qt5::Widgets)

include_directories(..)
qt5_add_resources(NEOVIM_RCC_SOURCES data.qrc)
if(WIN32)
	set(RES_FILE "data.rc")
	enable_language(RC)
endif()

add_executable(nvim-qt WIN32 main.cpp shell.cpp input.cpp errorwidget.cpp mainwindow.cpp
	${NEOVIM_RCC_SOURCES}
	${CMAKE_SOURCE_DIR}/third-party/konsole_wcwidth.cpp
	${RES_FILE})
target_link_libraries(nvim-qt ${QTLIBS} ${MSGPACK_LIBRARIES} neovim-qt)

add_executable(nvim-qt-widget EXCLUDE_FROM_ALL main.cpp shell.cpp input.cpp 
	${NEOVIM_RCC_SOURCES}
	${CMAKE_SOURCE_DIR}/third-party/konsole_wcwidth.cpp)
set_target_properties(nvim-qt-widget PROPERTIES COMPILE_DEFINITIONS NEOVIMQT_GUI_WIDGET )
target_link_libraries(nvim-qt-widget ${QTLIBS} ${MSGPACK_LIBRARIES} neovim-qt)

if(WIN32 AND NOT CMAKE_CROSSCOMPILING)
	include(WinDeployQt)
	WinDeployQt(TARGET nvim-qt COMPILER_RUNTIME INCLUDE_MODULES ${QTLIBS} EXCLUDE_MODULES webkit webkit2)
endif()

add_executable(fontinfo EXCLUDE_FROM_ALL fontinfo.cpp)
target_link_libraries(fontinfo Qt5::Gui)

