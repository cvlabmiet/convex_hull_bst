project(convex_hull)

cmake_minimum_required(VERSION 3.2)

# CGAL
find_package(CGAL REQUIRED)
include(${CGAL_USE_FILE})
include(CGAL_CreateSingleSourceCGALProgram)
include_directories(BEFORE "../../include")
include_directories(BEFORE "include")

# Warnings
if (MSVC)
   add_compile_options(/W4)
else()
   add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include_directories(algorithms)

add_executable(main charts/main.cpp)
target_link_libraries(main ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})

set_property(TARGET main PROPERTY CXX_STANDARD 14)

enable_testing()

add_executable(randomtest test/randomtesting.cpp)
target_link_libraries(randomtest ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})

add_test(Randomtest randomtest)
