include_directories(after .)

set(target mxqcore)
set(src mxq_queue.c)
set(src ${src} mxq_pthreadpool.c)

add_library(${target} SHARED ${src})
target_link_libraries(${target} pthread)

set_target_properties(${target} PROPERTIES SOVERSION 0 VERSION 0.1.2)

install(TARGETS ${target}
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin)

