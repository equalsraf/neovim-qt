set(QTLIBS Qt5::Network Qt5::Svg Qt5::Widgets)

include_directories(.. shellwidget)

if(WIN32)
	if (USE_STATIC_QT)
		add_definitions(-DUSE_STATIC_QT)
		link_directories(${CMAKE_PREFIX_PATH}/share/qt5/plugins/platforms)
        list(APPEND QTLIBS Qt5PlatformCompositorSupport qwindows)
	endif()
	set(RES_FILE "data.rc")
	enable_language(RC)
elseif(APPLE)
	set(ICON_PATH ${PROJECT_SOURCE_DIR}/third-party/neovim.icns)
	set_source_files_properties(${ICON_PATH} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

	set(MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/cmake/MacOSXBundleInfo.plist.in)
endif()

add_subdirectory(shellwidget)

include(GNUInstallDirs)
set(RUNTIME_PATH)

add_library(neovim-qt-gui STATIC)

target_sources(neovim-qt-gui
  PRIVATE
	app.cpp
	contextmenu.cpp
	errorwidget.cpp
	input.cpp
	mainwindow.cpp
	popupmenu.cpp
	popupmenumodel.cpp
	scrollbar.cpp
	shell.cpp
	tabline.cpp
	treeview.cpp
	${SRCS_PLATFORM}
    data.qrc
)

if(WIN32)
    target_sources(neovim-qt-gui
      PRIVATE
        arguments_qwindowgeometry.cpp
        input_win32.cpp
        printinfo_msgbox.cpp
    )
elseif(APPLE)
    target_sources(neovim-qt-gui
      PRIVATE
        arguments_qwindowgeometry.cpp
        input_mac.cpp
        printinfo_stdout.cpp
    )
else()
    target_sources(neovim-qt-gui
      PRIVATE
        arguments_geometry.cpp
        input_unix.cpp
        printinfo_stdout.cpp
    )
endif()


target_link_libraries(neovim-qt-gui
  PUBLIC
    ${QTLIBS}
    qshellwidget
    neovim-qt
)

if(APPLE)
	set_property(SOURCE app.cpp PROPERTY COMPILE_DEFINITIONS
		NVIM_QT_RELATIVE_RUNTIME_PATH="../Resources/runtime")
else()
	set_property(SOURCE app.cpp PROPERTY COMPILE_DEFINITIONS
		NVIM_QT_RELATIVE_RUNTIME_PATH="../share/nvim-qt/runtime")
endif()


add_executable(nvim-qt WIN32 MACOSX_BUNDLE)

target_sources(nvim-qt
  PRIVATE
    main.cpp
    data.qrc
	${RES_FILE}
    ${ICON_PATH}
)

target_link_libraries(nvim-qt PRIVATE neovim-qt-gui)

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

# Link Time Optimization, especially useful for unused auto-generated code.
# On Windows, MinGW may not support LTO.
# See https://github.com/neovim/neovim/pull/8654
if(POLICY CMP0069 AND NOT MINGW)
	cmake_policy(SET CMP0069 NEW)

	include(CheckIPOSupported)
	check_ipo_supported(RESULT IPO_SUPPORTED)
	if(IPO_SUPPORTED AND (NOT CMAKE_BUILD_TYPE MATCHES Debug))
		set_property(TARGET nvim-qt PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
	endif()
endif()

install(TARGETS nvim-qt DESTINATION ${CMAKE_INSTALL_BINDIR})
if(NOT APPLE)
	install(DIRECTORY runtime
		DESTINATION ${CMAKE_INSTALL_DATADIR}/nvim-qt/
		PATTERN "README.md" EXCLUDE)
	install(FILES ${PROJECT_SOURCE_DIR}/LICENSE
		DESTINATION ${CMAKE_INSTALL_DATADIR}/nvim-qt/)
	install(FILES nvim-qt.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
    install(FILES ${PROJECT_SOURCE_DIR}/third-party/neovim.png
			RENAME nvim-qt.png
			DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/192x192/apps/)
    install(FILES ${PROJECT_SOURCE_DIR}/third-party/neovim.svg
			RENAME nvim-qt.svg
			DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps/)
endif()

if(WIN32 AND NOT CMAKE_CROSSCOMPILING AND NOT DEFINED USE_STATIC_QT)
	include(WinDeployQt)
	WinDeployQt(TARGET nvim-qt COMPILER_RUNTIME INCLUDE_MODULES ${QTLIBS} EXCLUDE_MODULES webkit webkit2)
    install(
        DIRECTORY ${PROJECT_BINARY_DIR}/windeployqt/
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

# Generate help tags
add_custom_command(
	OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/runtime/doc/tags"
	COMMAND ${NEOVIM_EXEC} -i NONE -u NONE -e --headless
	-c "helptags ${CMAKE_CURRENT_SOURCE_DIR}/runtime/doc" -c "quit"
	COMMENT "Generating helptags")

add_custom_target(helptags ALL
	DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/runtime/doc/tags")
