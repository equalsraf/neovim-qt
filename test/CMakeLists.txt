set(QTLIBS Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Svg Qt${QT_VERSION_MAJOR}::Test)
set(QTLIBS_GUI Qt${QT_VERSION_MAJOR}::Widgets)
set(QTLIBS_TEST Qt${QT_VERSION_MAJOR}::Test)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_definitions(-DCMAKE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")
add_definitions(-DNVIM_QT_BINARY=\"$<TARGET_FILE:nvim-qt>\")
if (WIN32 AND USE_STATIC_QT)
	add_definitions(-DUSE_STATIC_QT)
endif ()

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

# Place all platform-specific test definitions here
if(WIN32)
	set(SRC_SHELL_PLATFORM tst_shell_win32.cpp)
elseif(APPLE)
	set(SRC_SHELL_PLATFORM tst_shell_mac.cpp)
else()
	set(SRC_SHELL_PLATFORM tst_shell_unix.cpp)
endif()

set(SOURCES_COMMON common.cpp)
set(SOURCES_COMMON_GUI
	common_gui.cpp
	mock_qsettings.cpp)

function(add_xtest SOURCE_NAME)
	add_executable(${SOURCE_NAME}
		${SOURCE_NAME}.cpp
		${SOURCES_COMMON}
		${ARGV1}
		${ARGV2})
	target_link_libraries(${SOURCE_NAME} ${QTLIBS} ${MSGPACK_LIBRARIES} neovim-qt)
	add_test(NAME ${SOURCE_NAME} COMMAND ${SOURCE_NAME} ${CTEST_EXE_ARGS})
	add_dependencies(check ${SOURCE_NAME})
endfunction()

function(add_xtest_gui SOURCE_NAME)
	if(${QT_VERSION_MAJOR} VERSION_GREATER_EQUAL 6)
		qt_add_resources(RCC_SOURCES data.qrc)
	else()
		qt5_add_resources(RCC_SOURCES data.qrc)
	endif()
	add_executable(${SOURCE_NAME}
		${RCC_SOURCES}
		${SOURCE_NAME}.cpp
		${SOURCES_COMMON}
		${SOURCES_COMMON_GUI}
		${SRC_SHELL_PLATFORM}
		${ARGV1} ${ARGV2})
	target_link_libraries(${SOURCE_NAME} ${QTLIBS} ${MSGPACK_LIBRARIES} neovim-qt Qt${QT_VERSION_MAJOR}::Widgets neovim-qt-gui)
	add_test(NAME ${SOURCE_NAME} COMMAND ${SOURCE_NAME} ${CTEST_EXE_ARGS}
		# Run GUI tests from source dir, they depend on src files
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
	add_dependencies(check ${SOURCE_NAME})
endfunction()

function(add_shared_test SOURCE_NAME)
	add_executable(${SOURCE_NAME} ${ARGV1} ${ARGV2} ${ARGV3})
	target_link_libraries(${SOURCE_NAME} Qt${QT_VERSION_MAJOR}::Network ${QTLIBS_TEST} ${QTLIBS_GUI} ${MSGPACK_LIBRARIES} neovim-qt)
	add_test(NAME ${SOURCE_NAME} COMMAND ${SOURCE_NAME})
	add_dependencies(check ${SOURCE_NAME})
endfunction()

add_xtest(tst_neovimobject)
add_xtest(tst_neovimconnector)
add_xtest(tst_callallmethods)
add_xtest(tst_encoding)
add_xtest(tst_msgpackiodevice)
add_xtest_gui(tst_shell)
add_xtest_gui(tst_main)
add_xtest_gui(tst_qsettings)

# Platform Specific Input Tests
add_xtest(tst_input_mac
	${CMAKE_SOURCE_DIR}/src/gui/input.cpp
	${CMAKE_SOURCE_DIR}/src/gui/input_mac.cpp)
add_xtest(tst_input_unix
	${CMAKE_SOURCE_DIR}/src/gui/input.cpp
	${CMAKE_SOURCE_DIR}/src/gui/input_unix.cpp)
add_xtest(tst_input_win32
	${CMAKE_SOURCE_DIR}/src/gui/input.cpp
	${CMAKE_SOURCE_DIR}/src/gui/input_win32.cpp)

# Platform Agnostic Input Tests
add_shared_test(tst_input_common_mac
	tst_input_common.cpp
	${CMAKE_SOURCE_DIR}/src/gui/input.cpp
	${CMAKE_SOURCE_DIR}/src/gui/input_mac.cpp)
add_shared_test(tst_input_common_unix
	tst_input_common.cpp
	${CMAKE_SOURCE_DIR}/src/gui/input.cpp
	${CMAKE_SOURCE_DIR}/src/gui/input_unix.cpp)
add_shared_test(tst_input_common_win32
	tst_input_common.cpp
	${CMAKE_SOURCE_DIR}/src/gui/input.cpp
	${CMAKE_SOURCE_DIR}/src/gui/input_win32.cpp)
