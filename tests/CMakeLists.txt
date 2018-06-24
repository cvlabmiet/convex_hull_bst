add_executable(randomtest randomtesting.cpp)
target_link_libraries(randomtest ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})

set_property(TARGET main PROPERTY CXX_STANDARD 14)
