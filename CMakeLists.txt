cmake_minimum_required(VERSION 3.1)
project(miraj)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.4 COMPONENTS system window graphics audio REQUIRED)

set(SOURCES
    src/main.cpp
    src/text/document.cpp
    src/text/vvdocument.cpp
)

add_executable(miraj ${SOURCES})
target_link_libraries(miraj ${SFML_LIBRARIES})
