target_link_libraries(${exe}
        libfreeglut.a
        libfreeglut_static.a
        opengl32
        glu32
        CGAL::CGAL
        )
