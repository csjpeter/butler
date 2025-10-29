cmake_minimum_required(VERSION 3.14)
project(butler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt settings
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# Output directories
# For multi-config generators (Visual Studio, Xcode), output goes to config-specific subdirs
# For single-config generators (Unix Makefiles, Ninja), output goes directly to build/obj
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/obj)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/obj)
# Also set per-configuration directories for multi-config generators
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/build/obj/${OUTPUTCONFIG})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/build/obj/${OUTPUTCONFIG})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR}/build/obj/${OUTPUTCONFIG})
endforeach()

# Platform-specific settings
if(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SQLITE3 REQUIRED sqlite3)
    include_directories(${SQLITE3_INCLUDE_DIRS})
    link_libraries(${SQLITE3_LIBRARIES})
elseif(WIN32)
    # On Windows, find SQLite3
    # When using vcpkg, the toolchain file should be set, which helps CMake find packages
    # Otherwise, we search in common locations
    find_path(SQLITE3_INCLUDE_DIR 
        NAMES sqlite3.h
        HINTS 
            "${CMAKE_PREFIX_PATH}/include"
            "$ENV{VCPKG_ROOT}/installed/x64-windows/include"
            "$ENV{VCPKG_INSTALLATION_PATH}/installed/x64-windows/include"
    )
    find_library(SQLITE3_LIBRARY 
        NAMES sqlite3
        HINTS 
            "${CMAKE_PREFIX_PATH}/lib"
            "$ENV{VCPKG_ROOT}/installed/x64-windows/lib"
            "$ENV{VCPKG_INSTALLATION_PATH}/installed/x64-windows/lib"
    )
    if(SQLITE3_INCLUDE_DIR AND SQLITE3_LIBRARY)
        include_directories(${SQLITE3_INCLUDE_DIR})
        set(SQLITE3_LIBRARIES ${SQLITE3_LIBRARY})
        message(STATUS "Found SQLite3: ${SQLITE3_LIBRARY}")
    else()
        message(FATAL_ERROR "SQLite3 not found. Please install SQLite3 or set CMAKE_PREFIX_PATH to include the SQLite3 installation directory.\n"
                "When using vcpkg, make sure to set CMAKE_TOOLCHAIN_FILE to the vcpkg toolchain file.\n"
                "SQLITE3_INCLUDE_DIR: ${SQLITE3_INCLUDE_DIR}\n"
                "SQLITE3_LIBRARY: ${SQLITE3_LIBRARY}")
    endif()
endif()

# Include directories
include_directories(
    libcsjp/devel
    libcsjp/core
    libcsjp/container
    libcsjp/system
    libcsjp/human
    src
    src/qt
)

# Compiler flags
if(MSVC)
    # MSVC-specific flags
    add_compile_options(
        /W3
        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Debug>:/Zi>
    )
    # Disable specific warnings for Qt and third-party code
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    # GCC/Clang flags
    add_compile_options(
        -Wno-expansion-to-defined
        -Wno-deprecated-copy
        -Wno-implicit-fallthrough
        -Wno-unused-function
        $<$<CONFIG:Debug>:-Og>
        $<$<CONFIG:Debug>:-g3>
        $<$<CONFIG:Debug>:-ggdb3>
    )
endif()

# Source files
set(SOURCES
    libcsjp/container/csjp_json.cpp
    libcsjp/core/csjp_astr.cpp
    libcsjp/core/csjp_file.cpp
    libcsjp/core/csjp_mutex.cpp
    libcsjp/core/csjp_str.cpp
    libcsjp/core/csjp_string.cpp
    libcsjp/devel/csjp_exception.cpp
    libcsjp/devel/csjp_logger.cpp
    libcsjp/devel/csjp_utility.cpp
    src/qt/butler_application.cpp
    src/qt/butler_config.cpp
    src/qt/butler_customview.cpp
    src/qt/butler_databases.cpp
    src/qt/butler_databasesview.cpp
    src/qt/butler_datamodel.cpp
    src/qt/butler_editaccountview.cpp
    src/qt/butler_editbrandview.cpp
    src/qt/butler_editcompanyview.cpp
    src/qt/butler_editdbdescview.cpp
    src/qt/butler_editinventoryview.cpp
    src/qt/butler_edititemview.cpp
    src/qt/butler_editpartnerview.cpp
    src/qt/butler_editpaymentview.cpp
    src/qt/butler_edittagview.cpp
    src/qt/butler_editwareview.cpp
    src/qt/butler_infoview.cpp
    src/qt/butler_kineticscroller.cpp
    src/qt/butler_listviews.cpp
    src/qt/butler_mainview.cpp
    src/qt/butler_pannview.cpp
    src/qt/butler_paymentqueryoptionsview.cpp
    src/qt/butler_paymentstatsview.cpp
    src/qt/butler_paymentview.cpp
    src/qt/butler_queryoptionsview.cpp
    src/qt/butler_statsview.cpp
    src/qt/butler_tagwidget.cpp
    src/qt/butler_texts.cpp
    src/qt/butler_widgets.cpp
    src/qt/main.cpp
    src/butler_dataclasses.cpp
    src/butler_sql_connection.cpp
)

# Platform-specific utility files
if(ANDROID)
    list(APPEND SOURCES libcsjp/devel/csjp_utility_android.cpp)
endif()

if(WIN32)
    list(APPEND SOURCES libcsjp/devel/csjp_utility_mxe.cpp)
endif()

# UNIX-specific system/network files (not used by Butler app, but included for completeness on Linux)
if(UNIX)
    list(APPEND SOURCES
        libcsjp/system/csjp_client.cpp
        libcsjp/system/csjp_daemon.cpp
        libcsjp/system/csjp_dup_socket.cpp
        libcsjp/system/csjp_epoll.cpp
        libcsjp/system/csjp_epoll_control.cpp
        libcsjp/system/csjp_http.cpp
        libcsjp/system/csjp_listener.cpp
        libcsjp/system/csjp_server.cpp
        libcsjp/system/csjp_signal.cpp
        libcsjp/system/csjp_socket.cpp
        libcsjp/system/csjp_websocket.cpp
    )
endif()

# Header files (for IDEs and MOC)
set(HEADERS
    src/qt/butler_abstract_table_model.h
    libcsjp/container/csjp_array.h
    libcsjp/container/csjp_bintree.h
    libcsjp/container/csjp_container.h
    libcsjp/container/csjp_ipc_block.h
    libcsjp/container/csjp_json.h
    libcsjp/container/csjp_owner_container.h
    libcsjp/container/csjp_ownerbintree.h
    libcsjp/container/csjp_pod_array.h
    libcsjp/container/csjp_ref_array.h
    libcsjp/container/csjp_reference_container.h
    src/qt/butler_listviews.h
    src/qt/butler_mainview.h
    src/qt/butler_pannview.h
    src/qt/butler_paymentqueryoptionsview.h
    src/qt/butler_paymentstatsview.h
    src/qt/butler_paymentview.h
    src/qt/butler_queryoptionsview.h
    src/qt/butler_statsview.h
    src/qt/butler_tagwidget.h
    src/qt/butler_texts.h
    src/qt/butler_widgets.h
    src/butler_conversions.h
    src/butler_dataclasses.h
    src/butler_datetime.h
    src/butler_macros.h
    src/butler_sql_connection.h
    src/butler_text.h
    src/butler_types.h
    src/config.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    ${SQLITE3_LIBRARIES}
)

# Install rules (optional)
# install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Enable testing
enable_testing()

# Add test subdirectories
add_subdirectory(libcsjp/container/test)
add_subdirectory(libcsjp/core/test)
add_subdirectory(libcsjp/devel/test)
# libcsjp/human tests require ICU - skip for now
# add_subdirectory(libcsjp/human/test)
# libcsjp/system tests use UNIX-specific networking code - skip on Windows
if(UNIX)
    add_subdirectory(libcsjp/system/test)
endif()
add_subdirectory(src/test)
add_subdirectory(src/qt/test)
