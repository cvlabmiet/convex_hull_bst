project(convex_hull)

cmake_minimum_required(VERSION 3.2)

if (MSVC)
   add_compile_options(/W4)
else()
   add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(main c++/main.cpp)
set_property(TARGET main PROPERTY CXX_STANDARD 14)
