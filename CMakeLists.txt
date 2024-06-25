cmake_minimum_required(VERSION 3.15)
project(ec_coding_project)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -DASIO_STANDALONE")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/gf-complete/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/jerasure/include/jerasure)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/jerasure/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/jerasure/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/gf-complete/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/lib)

add_executable(test_rs 
			   ${CMAKE_CURRENT_SOURCE_DIR}/src/common.cpp
			   ${CMAKE_CURRENT_SOURCE_DIR}/src/rs.cpp
			   ${CMAKE_CURRENT_SOURCE_DIR}/examples/test_rs.cpp)

add_executable(test_lrc 
			   ${CMAKE_CURRENT_SOURCE_DIR}/src/common.cpp
			   ${CMAKE_CURRENT_SOURCE_DIR}/src/rs.cpp
			   ${CMAKE_CURRENT_SOURCE_DIR}/src/lrc.cpp
			   ${CMAKE_CURRENT_SOURCE_DIR}/examples/test_lrc.cpp)

add_executable(test_pc 
			   ${CMAKE_CURRENT_SOURCE_DIR}/src/common.cpp
			   ${CMAKE_CURRENT_SOURCE_DIR}/src/rs.cpp
			   ${CMAKE_CURRENT_SOURCE_DIR}/src/pc.cpp
			   ${CMAKE_CURRENT_SOURCE_DIR}/examples/test_pc.cpp)

target_link_libraries(test_rs -lJerasure -lgf_complete)
target_link_libraries(test_lrc -lJerasure -lgf_complete)
target_link_libraries(test_pc -lJerasure -lgf_complete)

