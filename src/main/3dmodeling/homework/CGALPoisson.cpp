#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Surface_mesh_default_triangulation_3.h>
#include <CGAL/make_surface_mesh.h>
#include <CGAL/Implicit_surface_3.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/IO/facets_in_complex_2_to_triangle_mesh.h>
#include <CGAL/Poisson_reconstruction_function.h>
#include <CGAL/property_map.h>
#include <CGAL/IO/read_points.h>
#include <CGAL/compute_average_spacing.h>
#include <CGAL/Polygon_mesh_processing/distance.h>
#include <boost/iterator/transform_iterator.hpp>
#include <vector>
#include <fstream>
#include <CGAL/IO/output_surface_facets_to_polyhedron.h>
#include <CGAL/remove_outliers.h>

// Types
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::FT FT;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;
typedef std::pair<Point, Vector> Point_with_normal;
typedef CGAL::First_of_pair_property_map<Point_with_normal> Point_map;
typedef CGAL::Second_of_pair_property_map<Point_with_normal> Normal_map;
typedef Kernel::Sphere_3 Sphere;
typedef std::vector<Point_with_normal> PointList;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef CGAL::Poisson_reconstruction_function<Kernel> Poisson_reconstruction_function;
typedef CGAL::Surface_mesh_default_triangulation_3 STr;
typedef CGAL::Surface_mesh_complex_2_in_triangulation_3<STr> C2t3;
typedef CGAL::Implicit_surface_3<Kernel, Poisson_reconstruction_function> Surface_3;

class iterator;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"

int main() {
    const bool usePlyOutput = false;

    // Poisson options
    FT sm_angle = 5.0; // Min triangle angle in degrees.
    FT sm_radius = 10; // Max triangle size w.r.t. point set average spacing. 30->10
    FT sm_distance = 0.05; // Surface Approximation error w.r.t. point set average spacing.
    // Reads the point set baseDir in points[].
    // Note: read_points() requires an iterator over points
    // + property maps to access each point's position and normal.

    // Removes outliers using erase-remove idiom.
    // The Identity_property_map property map can be omitted here as it is the default value.
    const int nb_neighbors = 24; // considers 24 nearest neighbor points

    PointList points;
    if (!CGAL::IO::read_points(CGAL::data_file_path("buddha/cmvs-pc.ply"), std::back_inserter(points),
                               CGAL::parameters::point_map(Point_map())
                                       .normal_map(Normal_map()))) {
        std::cerr << "Error: cannot read baseDir input baseDir!" << std::endl;
        return EXIT_FAILURE;
    }

    // Creates implicit function from the read points using the default solver.
    // Note: this method requires an iterator over points
    // + property maps to access each point's position and normal.
    Poisson_reconstruction_function function(points.begin(), points.end(), Point_map(), Normal_map());
    // Computes the Poisson indicator function f()
    // at each vertex of the triangulation.
    if (!function.compute_implicit_function())
        return EXIT_FAILURE;
    // Computes average spacing
    FT average_spacing = CGAL::compute_average_spacing<CGAL::Sequential_tag>
            (points, 6 /* knn = 1 ring */,
             CGAL::parameters::point_map(Point_map()));

    // Gets one point inside the implicit surface
    // and computes implicit function bounding sphere radius.
    Point inner_point = function.get_inner_point();
    Sphere bsphere = function.bounding_sphere();
    FT radius = std::sqrt(bsphere.squared_radius());
    // Defines the implicit surface: requires defining a
    // conservative bounding sphere centered at inner point.
    FT sm_sphere_radius = 5.0 * radius;
    FT sm_dichotomy_error = sm_distance * average_spacing / 1000.0; // Dichotomy error must be << sm_distance
    Surface_3 surface(function,
                      Sphere(inner_point, sm_sphere_radius * sm_sphere_radius),
                      sm_dichotomy_error / sm_sphere_radius);
    // Defines surface mesh generation criteria
    CGAL::Surface_mesh_default_criteria_3<STr> criteria(sm_angle,  // Min triangle angle (degrees)
                                                        sm_radius * average_spacing,  // Max triangle size
                                                        sm_distance * average_spacing); // Approximation error
    // Generates surface mesh with manifold option
    STr tr; // 3D Delaunay triangulation for surface mesh generation
    // SurfaceMeshComplex_2InTriangulation_3, a class template from the Surface Mesh Generation package
    C2t3 c2t3(tr); // 2D complex in 3D Delaunay triangulation
    CGAL::make_surface_mesh(c2t3,                                 // reconstructed mesh
                            surface,                              // implicit surface
                            criteria,                             // meshing criteria
                            CGAL::Manifold_with_boundary_tag());  // require manifold mesh
    if (tr.number_of_vertices() == 0)
        return EXIT_FAILURE;
    // saves reconstructed surface mesh
    std::stringstream fileNameStr;
    fileNameStr << "buddha/poisson-mesh-" << sm_angle << "-" << sm_radius << "-" << sm_distance;
    if (usePlyOutput)
        fileNameStr << ".ply";
    else
        fileNameStr << ".off";

    std::ofstream out(fileNameStr.str());
    Polyhedron output_mesh;

    if (usePlyOutput)
        CGAL::output_surface_facets_to_polyhedron(c2t3, output_mesh);
    else
        CGAL::facets_in_complex_2_to_triangle_mesh(c2t3, output_mesh);

    out << output_mesh;
    // computes the approximation error of the reconstruction
    double max_dist =
            CGAL::Polygon_mesh_processing::approximate_max_distance_to_point_set
                    (output_mesh,
                     CGAL::make_range(boost::make_transform_iterator
                                              (points.begin(), CGAL::Property_map_to_unary_function<Point_map>()),
                                      boost::make_transform_iterator
                                              (points.end(), CGAL::Property_map_to_unary_function<Point_map>())),
                     4000);
    std::cout << "Max distance to point_set: " << max_dist << std::endl;
    return EXIT_SUCCESS;
}

#pragma clang diagnostic pop
