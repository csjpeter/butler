cmake_minimum_required(VERSION 3.14)

project(test_string_split)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt settings
set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Test REQUIRED)

# Enable testing
enable_testing()

# Add test executable
add_executable(test_string_split test_string_split.cpp)

target_link_libraries(test_string_split
    Qt5::Core
    Qt5::Test
)

# Add test to CTest
add_test(NAME test_string_split COMMAND test_string_split)
