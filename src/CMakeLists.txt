
add_executable(neovimqtgen neovimqtgen.cpp function.cpp util.cpp)
set_target_properties(neovimqtgen PROPERTIES COMPILE_FLAGS -DNEOVIMQT_NO_AUTO=1)
target_link_libraries(neovimqtgen Qt5::Network ${MSGPACK_LIBRARIES})

add_custom_target(neovim_auto
	${NEOVIM_EXEC} --api-info | ${CMAKE_BINARY_DIR}/src/neovimqtgen ${CMAKE_CURRENT_SOURCE_DIR}/auto/
	DEPENDS neovimqtgen
	)

set(NEOVIM_QT_SOURCES neovimconnector.cpp function.cpp auto/neovim.cpp util.cpp)
add_library(neovim-qt ${NEOVIM_QT_SOURCES})
target_link_libraries(neovim-qt Qt5::Network ${MSGPACK_LIBRARIES})

