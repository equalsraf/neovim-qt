include_directories(..)

if (WIN32 AND USE_STATIC_QT)
	add_definitions(-DUSE_STATIC_QT)
endif ()

function(add_xtest SOURCE_NAME)
	add_executable(${SOURCE_NAME} ${SOURCE_NAME}.cpp)
	target_link_libraries(${SOURCE_NAME} Qt5::Widgets Qt5::Test qshellwidget)
	add_test(NAME ${SOURCE_NAME} COMMAND ${SOURCE_NAME})
endfunction()

add_xtest(test_cell)
add_xtest(test_shellcontents)
add_xtest(test_shellwidget)
add_xtest(bench_scroll)
add_xtest(bench_cell)
