include_directories(${CONVEX_HULL_BST}/algorithms)

add_executable(main main.cpp)
target_link_libraries(main ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})

set_property(TARGET main PROPERTY CXX_STANDARD 14)
