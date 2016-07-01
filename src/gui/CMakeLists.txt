set(QTLIBS Qt5::Network Qt5::Widgets)

include_directories(..)
qt5_add_resources(NEOVIM_RCC_SOURCES data.qrc)
if(WIN32)
	set(RES_FILE "data.rc")
	enable_language(RC)
elseif(APPLE)
	set(ICON_PATH ${PROJECT_SOURCE_DIR}/third-party/neovim.icns)
	set_source_files_properties(${ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

	set(MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/cmake/MacOSXBundleInfo.plist.in)
endif()

add_subdirectory(shellwidget)

include(GNUInstallDirs)
set(RUNTIME_PATH )
add_library(neovim-qt-gui shell.cpp input.cpp errorwidget.cpp mainwindow.cpp app.cpp
	popupmenu.cpp popupwidgetitem.cpp
	${CMAKE_SOURCE_DIR}/third-party/konsole_wcwidth.cpp
	${NEOVIM_RCC_SOURCES})
target_link_libraries(neovim-qt-gui qshellwidget neovim-qt)

if(NOT APPLE)
	set_property(SOURCE main.cpp PROPERTY COMPILE_DEFINITIONS
		NVIM_QT_RUNTIME_PATH="${CMAKE_INSTALL_FULL_DATADIR}/nvim-qt/runtime")
endif()

add_executable(nvim-qt WIN32 MACOSX_BUNDLE main.cpp
	${NEOVIM_RCC_SOURCES}
	${RES_FILE}
	${ICON_PATH})

target_link_libraries(nvim-qt ${QTLIBS} ${MSGPACK_LIBRARIES} neovim-qt-gui)

if(APPLE)
	add_custom_command(TARGET nvim-qt COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/runtime/
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/nvim-qt.app/Contents/Resources/runtime
		COMMENT "Copying runtime into bundle")
#	TODO: Bundle Qt, currently conflicts with brew Qt
#	add_custom_command(TARGET nvim-qt COMMAND macdeployqt
#		ARGS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/nvim-qt.app
#		COMMENT "Bundling Qt libraries")
endif()

install(TARGETS nvim-qt DESTINATION ${CMAKE_INSTALL_BINDIR})
if(NOT APPLE)
	install(DIRECTORY runtime
		DESTINATION ${CMAKE_INSTALL_DATADIR}/nvim-qt/)
	install(FILES nvim-qt.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
	install(FILES ${CMAKE_SOURCE_DIR}/third-party/neovim.png
			RENAME nvim-qt.png
			DESTINATION ${CMAKE_INSTALL_DATADIR}/pixmaps/)
endif()

if(WIN32 AND NOT CMAKE_CROSSCOMPILING)
	if(Qt5Core_VERSION VERSION_LESS 5.3)
		message(WARNING "Cannot use windeployqt in Qt 5.2 and earlier")
	else()
		include(WinDeployQt)
		WinDeployQt(TARGET nvim-qt COMPILER_RUNTIME INCLUDE_MODULES ${QTLIBS} EXCLUDE_MODULES webkit webkit2)
		install(DIRECTORY ${PROJECT_BINARY_DIR}/windeployqt/
			DESTINATION ${CMAKE_INSTALL_BINDIR})
	endif()
endif()
