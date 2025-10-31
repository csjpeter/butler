# Source tests

# Enable Qt for src tests
set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core REQUIRED)

# Helper macro to create test executables
macro(add_butler_test test_name sources)
    add_executable(${test_name} ${sources})
    target_link_libraries(${test_name} 
        Qt5::Core
        ${SQLITE3_LIBRARIES}
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endmacro()

# Common source files for butler tests
set(BUTLER_COMMON_SOURCES
    ${CMAKE_SOURCE_DIR}/libcsjp/container/csjp_json.cpp
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
add_butler_test(test_src_dataclasses "dataclasses.cpp;${CMAKE_SOURCE_DIR}/src/butler_dataclasses.cpp;${CMAKE_SOURCE_DIR}/src/butler_sql_connection.cpp;${BUTLER_COMMON_SOURCES}")
