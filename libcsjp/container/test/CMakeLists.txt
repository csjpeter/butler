# Container tests

# Helper macro to create test executables
macro(add_csjp_test test_name sources)
    add_executable(${test_name} ${sources})
    target_link_libraries(${test_name} ${SQLITE3_LIBRARIES})
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
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
add_csjp_test(test_container_array "array.cpp;${CSJP_COMMON_SOURCES}")
add_csjp_test(test_container_bintree "bintree.cpp;${CSJP_COMMON_SOURCES}")
add_csjp_test(test_container_container "container.cpp;${CSJP_COMMON_SOURCES}")
add_csjp_test(test_container_json "json.cpp;${CMAKE_SOURCE_DIR}/libcsjp/container/csjp_json.cpp;${CSJP_COMMON_SOURCES}")
add_csjp_test(test_container_pod_array "pod_array.cpp;${CSJP_COMMON_SOURCES}")
add_csjp_test(test_container_ref_array "ref_array.cpp;${CSJP_COMMON_SOURCES}")
add_csjp_test(test_container_sorter_container "sorter_container.cpp;${CSJP_COMMON_SOURCES}")
