cmake_minimum_required(VERSION 3.30)
project(avmcord)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_executable(avmcord src/main.cpp
        src/env.cpp
        include/env.h
        src/join.cpp
        include/join.h
        include/string_hash.h)

find_package(DPP REQUIRED)

target_link_libraries(${PROJECT_NAME}
    ${DPP_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${DPP_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
