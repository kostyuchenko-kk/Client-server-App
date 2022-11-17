
cmake_minimum_required (VERSION 3.8)
project ("Client")
list(APPEND CMAKE_CXX_FLAGS "-pthread -std=c++11 ${CMAKE_CXX_FLAGS}")

add_executable (Client  "client.h" "client.cpp" "main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Client PROPERTY CXX_STANDARD 20)
endif()


