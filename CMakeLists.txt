cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(oscillator src/oscillator.cpp)

list(APPEND LIBRARIES)

target_include_directories(oscillator
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(oscillator
    PRIVATE
        ${LIBRARIES}
    
    PUBLIC
        ${LIBRARIES}
    
    INTERFACE
        ${LIBRARIES}
)
