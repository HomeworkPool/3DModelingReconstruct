#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/compute_average_spacing.h>
#include <CGAL/pca_estimate_normals.h>
#include <CGAL/mst_orient_normals.h>
#include <CGAL/property_map.h>
#include <CGAL/IO/read_points.h>
#include <utility> // defines std::pair
#include <list>
#include <fstream>
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

// Types
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;
// Point with normal vector stored in a std::pair.
typedef std::pair<Point, Vector> PointVectorPair;
// Concurrency
typedef CGAL::Parallel_if_available_tag Concurrency_tag;
int main(int argc, char*argv[])
{
    const std::string fname = (argc>1) ? argv[1] : CGAL::data_file_path("buddha/cmvs-pc-no-normals.ply");
    // Reads a point set baseDir in points[].
    std::list<PointVectorPair> points;
    if(!CGAL::IO::read_points(fname,
                              std::back_inserter(points),
                              CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>())))
    {
        std::cerr << "Error: cannot read baseDir " << fname<< std::endl;
        return EXIT_FAILURE;
    }
    // Estimates normals direction.
    // Note: pca_estimate_normals() requiresa range of points
    // as well as property maps to access each point's position and normal.
    const int nb_neighbors = 18; // K-nearest neighbors = 3 rings
    if (argc > 2 && std::strcmp(argv[2], "-r") == 0) // Use a fixed neighborhood radius
    {
        // First compute a spacing using the K parameter
        double spacing
                = CGAL::compute_average_spacing<Concurrency_tag>
                        (points, nb_neighbors,
                         CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>()));
        // Then, estimate normals with a fixed radius
        CGAL::pca_estimate_normals<Concurrency_tag>
                (points,
                 0, // when using a neighborhood radius, K=0 means no limit on the number of neighbors returns
                 CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>())
                         .normal_map(CGAL::Second_of_pair_property_map<PointVectorPair>())
                         .neighbor_radius(2. * spacing)); // use 2*spacing as neighborhood radius
    }
    else // Use a fixed number of neighbors
    {
        CGAL::pca_estimate_normals<Concurrency_tag>
                (points, nb_neighbors,
                 CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>())
                         .normal_map(CGAL::Second_of_pair_property_map<PointVectorPair>()));
    }
    // Orients normals.
    // Note: mst_orient_normals() requires a range of points
    // as well as property maps to access each point's position and normal.
    std::list<PointVectorPair>::iterator unoriented_points_begin =
            CGAL::mst_orient_normals(points, nb_neighbors,
                                     CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>())
                                             .normal_map(CGAL::Second_of_pair_property_map<PointVectorPair>()));
    // Optional: delete points with an unoriented normal
    // if you plan to call a reconstruction algorithm that expects oriented normals.
    points.erase(unoriented_points_begin, points.end());

    // saves reconstructed surface mesh
    // Saves point set.
    if(!CGAL::IO::write_points("buddha/cmvs-pc.ply", points,
                               CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>())
                                       .normal_map(CGAL::Second_of_pair_property_map<PointVectorPair>())
                                       .stream_precision(17)))
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
