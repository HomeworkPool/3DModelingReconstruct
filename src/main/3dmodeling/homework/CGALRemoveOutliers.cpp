#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/property_map.h>
#include <CGAL/compute_average_spacing.h>
#include <CGAL/remove_outliers.h>
#include <CGAL/IO/read_points.h>
#include <CGAL/IO/write_points.h>
#include <vector>
#include <fstream>
#include <iostream>
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

// types
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;
typedef std::pair<Point, Vector> Point_with_normal;
typedef CGAL::First_of_pair_property_map<Point_with_normal> Point_map;
typedef CGAL::Second_of_pair_property_map<Point_with_normal> Normal_map;

int main(int argc, char *argv[]) {
    const std::string fname = (argc > 1) ? argv[1] : CGAL::data_file_path("buddha/cmvs-pc-chaos.ply");
    // Reads a point set file in points[].
    // The Identity_property_map property map can be omitted here as it is the default value.
    std::vector<Point> points;
    if (!CGAL::IO::read_points(fname, std::back_inserter(points),
                               CGAL::parameters::point_map(CGAL::Identity_property_map<Point>()))) {
        std::cerr << "Error: cannot read file " << fname << std::endl;
        return EXIT_FAILURE;
    }
    // Removes outliers using erase-remove idiom.
    // The Identity_property_map property map can be omitted here as it is the default value.
    const int nb_neighbors = 24; // considers 24 nearest neighbor points
    // Estimate scale of the point set with average spacing
    const double average_spacing = CGAL::compute_average_spacing<CGAL::Sequential_tag>(points, nb_neighbors);
    // FIRST OPTION //
    // I don't know the ratio of outliers present in the point set
    std::vector<Point>::iterator first_to_remove
            = CGAL::remove_outliers<CGAL::Parallel_if_available_tag>
                    (points,
                     nb_neighbors,
                     CGAL::parameters::threshold_percent(100.). // No limit on the number of outliers to remove
                             threshold_distance(2. *
                                                average_spacing)); // Point with distance above 2*average_spacing are considered outliers
    std::cerr << (100. * std::distance(first_to_remove, points.end()) / static_cast<double>(points.size()))
              << "% of the points are considered outliers when using a distance threshold of "
              << 2. * average_spacing << std::endl;
    // SECOND OPTION //
    // I know the ratio of outliers present in the point set
    const double removed_percentage = 3.0; // percentage of points to remove
    points.erase(CGAL::remove_outliers<CGAL::Parallel_if_available_tag>
                         (points,
                          nb_neighbors,
                          CGAL::parameters::threshold_percent(removed_percentage). // Minimum percentage to remove
                                  threshold_distance(0.)), // No distance threshold (can be omitted)
                 points.end());
    // Optional: after erase(), use Scott Meyer's "swap trick" to trim excess capacity
    std::vector<Point>(points).swap(points);

    std::cerr << "Removed " << removed_percentage << "% of the points" << std::endl;
    std::cerr << "Final point set size: " << points.size() << std::endl;

    // Saves point set.
    CGAL::IO::write_points("buddha/cmvs-pc-no-normals.ply", points);

    return EXIT_SUCCESS;
}
