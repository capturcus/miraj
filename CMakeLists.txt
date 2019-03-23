cmake_minimum_required(VERSION 3.1)
project(miraj)

# SET(CMAKE_CXX_COMPILER clang++-6.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.4 COMPONENTS system window graphics audio REQUIRED)

find_package(jsoncpp REQUIRED)
include_directories(${JsonCpp_INCLUDE_DIR} ${SFML_INCLUDE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

file(GLOB miraj_SRC
    "src/structures/*.cpp"
    "src/appearance/*.cpp"
    "src/lexer/*.cpp"
    "src/parser/*.cpp"
    "src/*.cpp"
    )

add_executable(miraj ${miraj_SRC})
if(JsonCpp_LIBRARY)
else()
    set(JsonCpp_LIBRARY jsoncpp)
endif()
target_link_libraries(miraj ${SFML_LIBRARIES} ${JsonCpp_LIBRARY})
# target_link_libraries(miraj ${SFML_LIBRARIES} jsoncpp)
