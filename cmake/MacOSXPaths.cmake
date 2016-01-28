# Mac OS X: add homebrew, macports and fink paths, including homebrew Qt and zlib
if(APPLE)
	message(STATUS "Enabling build support for Mac OS X")

	set(CMAKE_PREFIX_PATH  "/usr/local/lib;/usr/local/opt/qt5;/opt/local/lib;/sw/lib;${CMAKE_PREFIX_PATH}")

	set(CMAKE_INCLUDE_PATH "/usr/local/include;/usr/local/opt/qt5/include;/usr/local/opt/zlib;/opt/local/include;/sw/include;${CMAKE_INCLUDE_PATH}")
	set(CMAKE_LIBRARY_PATH "/usr/local/lib;/usr/local/opt/qt5/lib;/usr/local/opt/zlib;/opt/local/lib;/sw/lib;${CMAKE_LIBRARY_PATH}")
	set(CMAKE_PROGRAM_PATH "/usr/local/bin;/usr/local/opt/qt5/bin;/opt/local/bin;/sw/bin;${CMAKE_PROGRAM_PATH}")

	include_directories("/usr/local/include" "/usr/local/opt/qt5/include" "/usr/local/opt/zlib/include" "/opt/local/include" "/sw/include")

	# only include dirs that exist or warnings are generated
	foreach(link_dir "/usr/local/lib" "/usr/local/opt/qt5/lib" "/usr/local/opt/zlib/lib" "/opt/local/lib" "/sw/lib")
		if(EXISTS ${link_dir} AND IS_DIRECTORY ${link_dir})
			link_directories(${link_dir})
		endif()
	endforeach()
endif()
