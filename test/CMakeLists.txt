cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 23)
project(jpp_test)
add_executable(jpp_test test.cc)
file(COPY json/ DESTINATION json/)
target_include_directories(jpp_test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")