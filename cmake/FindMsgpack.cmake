include(CheckLibraryExists)

find_path(MSGPACK_INCLUDE_DIR
    NAMES msgpack.hpp
)

find_library(MSGPACK_LIBRARY
    NAMES msgpack libmsgpack.a
)

mark_as_advanced(MSGPACK_INCLUDE_DIR MSGPACK_LIBRARY)
set(MSGPACK_LIBRARIES ${MSGPACK_LIBRARY})
set(MSGPACK_INCLUDE_DIRS ${MSGPACK_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Msgpack DEFAULT_MSG
	MSGPACK_LIBRARY MSGPACK_INCLUDE_DIR)
