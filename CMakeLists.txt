cmake_minimum_required(VERSION 3.15)
project(ec_coding_project)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -DASIO_STANDALONE")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/gf-complete/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/jerasure/include/jerasure)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/jerasure/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/jerasure/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/gf-complete/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

# set(SOURCES main.cpp)
add_executable(main 
			   ${CMAKE_CURRENT_SOURCE_DIR}/src/common.cpp
			   ${CMAKE_CURRENT_SOURCE_DIR}/src/rs.cpp
			   ${CMAKE_CURRENT_SOURCE_DIR}/src/lrc.cpp
			   ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_link_libraries(main -lJerasure -lgf_complete)

