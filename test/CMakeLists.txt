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

function(add_xtest SOURCE_NAME)
	add_executable(${SOURCE_NAME} ${SOURCE_NAME}.cpp ${ARGV1} ${ARGV2})
	target_link_libraries(${SOURCE_NAME} ${QTLIBS} ${MSGPACK_LIBRARIES} neovim-qt)
	add_test(NAME ${SOURCE_NAME} COMMAND ${SOURCE_NAME})
	add_dependencies(check ${SOURCE_NAME})
endfunction()

function(add_xtest_gui SOURCE_NAME)
	add_executable(${SOURCE_NAME} ${SOURCE_NAME}.cpp)
	target_link_libraries(${SOURCE_NAME} ${QTLIBS} ${MSGPACK_LIBRARIES} neovim-qt ${QTLIBS_GUI} neovim-qt-gui)
	add_test(NAME ${SOURCE_NAME} COMMAND ${SOURCE_NAME}
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
