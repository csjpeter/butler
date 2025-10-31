# Devel tests

# Helper macro to create test executables
macro(add_csjp_test test_name sources)
    add_executable(${test_name} ${sources})
    target_link_libraries(${test_name} ${SQLITE3_LIBRARIES})
    add_test(NAME ${test_name} COMMAND ${test_name})
endmacro()

# Common source files for tests
set(CSJP_COMMON_SOURCES
    ${CMAKE_SOURCE_DIR}/libcsjp/core/csjp_astr.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/core/csjp_string.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/core/csjp_str.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/devel/csjp_exception.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/devel/csjp_logger.cpp
    ${CMAKE_SOURCE_DIR}/libcsjp/devel/csjp_utility.cpp
)

# Add test executables
add_csjp_test(test_devel_exception "exception.cpp;${CSJP_COMMON_SOURCES}")
