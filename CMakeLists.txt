project(convex_hull)

cmake_minimum_required(VERSION 3.2)

if (MSVC)
   add_compile_options(/W4)
else()
   add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(ALGO_DIR algorithms)

include_directories(${ALGO_DIR})
set(ALGO_HEADERS ${ALGO_DIR}/bsthull.h ${ALGO_DIR}/graham.h ${ALGO_DIR}/point.h ${ALGO_DIR}/chan.h)

add_executable(main charts/main.cpp ${ALGO_HEADERS})

set_property(TARGET main PROPERTY CXX_STANDARD 14)

enable_testing()
add_executable(randomtest test/randomtesting.cpp ${ALGO_HEADERS})
add_test(Randomtest randomtest)
