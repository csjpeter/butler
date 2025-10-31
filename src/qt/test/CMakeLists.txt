# Qt source tests

# Enable Qt for tests
set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# Helper macro to create test executables
macro(add_butler_qt_test test_name sources)
    add_executable(${test_name} ${sources})
    target_link_libraries(${test_name} 
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        ${SQLITE3_LIBRARIES}
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
endmacro()

# Common source files for butler Qt tests
set(BUTLER_QT_COMMON_SOURCES
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
add_butler_qt_test(test_qt_config "config.cpp;${CMAKE_SOURCE_DIR}/src/qt/butler_config.cpp;${CMAKE_SOURCE_DIR}/src/qt/butler_texts.cpp;${BUTLER_QT_COMMON_SOURCES}")
