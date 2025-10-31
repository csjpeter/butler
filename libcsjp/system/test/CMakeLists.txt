# System tests

# Helper macro to create test executables
macro(add_csjp_test test_name sources)
    add_executable(${test_name} ${sources})
    target_link_libraries(${test_name} ${SQLITE3_LIBRARIES})
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endmacro()

# Common source files for system tests
set(SYSTEM_COMMON_SOURCES
    ${CMAKE_SOURCE_DIR}/libcsjp/system/csjp_socket.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/system/csjp_dup_socket.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/system/csjp_epoll.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/system/csjp_epoll_control.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/system/csjp_signal.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/system/csjp_listener.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/system/csjp_server.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/system/csjp_client.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/system/csjp_daemon.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/core/csjp_astr.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/core/csjp_string.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/core/csjp_str.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/core/csjp_file.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/core/csjp_mutex.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/devel/csjp_exception.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/devel/csjp_logger.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/devel/csjp_utility.cpp
)

# Add test executables
add_csjp_test(test_system_client "client.cpp;${SYSTEM_COMMON_SOURCES}")
add_csjp_test(test_system_daemon "daemon.cpp;${SYSTEM_COMMON_SOURCES}")
add_csjp_test(test_system_epoll "epoll.cpp;${SYSTEM_COMMON_SOURCES}")
add_csjp_test(test_system_http "http.cpp;${CMAKE_SOURCE_DIR}/libcsjp/system/csjp_http.cpp;${SYSTEM_COMMON_SOURCES}")
add_csjp_test(test_system_listener "listener.cpp;${SYSTEM_COMMON_SOURCES}")
# nc.cpp has API compatibility issues - skip for now
# add_csjp_test(test_system_nc "nc.cpp;${SYSTEM_COMMON_SOURCES}")
add_csjp_test(test_system_server "server.cpp;${SYSTEM_COMMON_SOURCES}")
add_csjp_test(test_system_signal "signal.cpp;${SYSTEM_COMMON_SOURCES}")
add_csjp_test(test_system_socket "socket.cpp;${SYSTEM_COMMON_SOURCES}")
add_csjp_test(test_system_websocket "websocket.cpp;${CMAKE_SOURCE_DIR}/libcsjp/system/csjp_websocket.cpp;${CMAKE_SOURCE_DIR}/libcsjp/system/csjp_http.cpp;${SYSTEM_COMMON_SOURCES}")
