cmake_minimum_required(VERSION 3.1)
project(miraj)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.4 COMPONENTS system window graphics audio REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

file(GLOB miraj_SRC
    "src/text/*.cpp"
    "src/lexer/*.cpp"
    "src/parser/*.cpp"
    "src/*.cpp"
    )

add_executable(miraj ${miraj_SRC})
target_link_libraries(miraj ${SFML_LIBRARIES} libjsoncpp)
