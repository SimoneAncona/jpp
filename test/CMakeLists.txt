set(CMAKE_CXX_STANDARD 20)
project(jpp_test)
add_executable(jpp_test test.cc)

if(UNIX AND ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    set(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10)
endif()

file(COPY json/ DESTINATION json/)
target_include_directories(jpp_test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")