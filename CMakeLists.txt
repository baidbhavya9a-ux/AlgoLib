cmake_minimum_required(VERSION 3.21)

project(AlgoLib VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

# AlgoLib GUI Application
qt_add_executable(AlgoLibApp
    app/main.cpp
    src/visualizer.cpp
    src/code_panel.cpp
    src/compressx.cpp
    src/graphkit.cpp
    src/graph_visualizer.cpp
    src/dplab.cpp
)

set_target_properties(AlgoLibApp PROPERTIES
    AUTOMOC OFF
)

target_include_directories(AlgoLibApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(AlgoLibApp PRIVATE
    Qt6::Widgets
)

# Automated Academic C++ Test Suite
add_executable(AlgoLibTests
    tests/test_main.cpp
    src/compressx.cpp
    src/graphkit.cpp
    src/dplab.cpp
)

target_include_directories(AlgoLibTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(AlgoLibTests PRIVATE
    Qt6::Widgets
)
