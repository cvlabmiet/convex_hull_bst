project(convex_hull_bst)

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

# Algorithms include
include_directories(${convex_hull_bst_SOURCE_DIR}/algorithms)

# Directories
add_subdirectory(charts)
add_subdirectory(tests)

# Tests
enable_testing()
add_test(Randomtest tests/randomtest)
