include_directories(after .)

set(target test_queue)
set(src test_queue.c)

add_executable(${target} ${src})

target_link_libraries(${target} mxqcore)

set(target test_pthreadpool)
set(src test_pthreadpool.c)

add_executable(${target} ${src})

target_link_libraries(${target} mxqcore)
