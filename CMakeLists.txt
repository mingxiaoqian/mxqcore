CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(mxqcore)

set(CMAKE_MACOSX_RPATH 1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -std=gnu99 -D_SVID_SOURCE")

include_directories(after include)

if (MXQ_ROOT)
	INCLUDE_DIRECTORIES(after ${MXQ_ROOT}/include)
	LINK_DIRECTORIES(${MXQ_ROOT}/lib)
endif ()

add_subdirectory(src)
add_subdirectory(test)

install (FILES include/list.h DESTINATION include)
install (FILES include/mxq.h DESTINATION include)

