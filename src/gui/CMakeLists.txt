set(QTLIBS Qt5::Network Qt5::Widgets)

include_directories(..)
qt5_add_resources(NEOVIM_RCC_SOURCES data.qrc)
if(WIN32)
	set(RES_FILE "data.rc")
	enable_language(RC)
endif()

add_subdirectory(shellwidget)

include(GNUInstallDirs)
set(RUNTIME_PATH )
add_library(neovim-qt-gui shell.cpp input.cpp errorwidget.cpp mainwindow.cpp
	${CMAKE_SOURCE_DIR}/third-party/konsole_wcwidth.cpp
	${NEOVIM_RCC_SOURCES})
target_link_libraries(neovim-qt-gui qshellwidget neovim-qt)

set_property(SOURCE main.cpp PROPERTY COMPILE_DEFINITIONS
	NVIM_QT_RUNTIME="${CMAKE_INSTALL_FULL_DATADIR}/nvim-qt/runtime")
add_executable(nvim-qt WIN32 main.cpp
	${NEOVIM_RCC_SOURCES}
	${RES_FILE})
target_link_libraries(nvim-qt ${QTLIBS} ${MSGPACK_LIBRARIES} neovim-qt-gui)

if(WIN32 AND NOT CMAKE_CROSSCOMPILING)
	include(WinDeployQt)
	WinDeployQt(TARGET nvim-qt COMPILER_RUNTIME INCLUDE_MODULES ${QTLIBS} EXCLUDE_MODULES webkit webkit2)
endif()

install(TARGETS nvim-qt DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES nvim-qt.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(FILES ${CMAKE_SOURCE_DIR}/third-party/neovim.png
        RENAME nvim-qt.png
        DESTINATION ${CMAKE_INSTALL_DATADIR}/pixmaps/)
install(DIRECTORY runtime
        DESTINATION ${CMAKE_INSTALL_DATADIR}/nvim-qt/)
