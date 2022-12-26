target_link_libraries(${exe}
        opengl32
        glu32
        CGAL::CGAL
        Eigen3::Eigen
        ${OPENMESH_LIBRARIES}
        )
target_include_directories(${exe} PRIVATE ${OPENMESH_INCLUDE_DIRS})
