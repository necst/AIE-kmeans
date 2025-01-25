#include "kmeans_kernel.h"
#include "common.h"
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "aie_api/utils.hpp"

aie::vector<float, MAX_CLUSTERS> euclidean_distance(aie::vector<float, MAX_CLUSTERS> clusters_x, aie::vector<float, MAX_CLUSTERS> clusters_y, int32_t num_clusters, Point point);
int32_t assignment_function(aie::vector<float, MAX_CLUSTERS> distances, int32_t num_clusters);

void kmeans_function(input_stream<int32_t> *restrict input, output_stream<float> *restrict output)
{
    // Read the number of clusters and points
    aie::vector<int32_t, 8> val_in = readincr_v<8>(input);
    aie::vector<float, 8> val_fl;
    int32_t num_clusters = (int32_t) val_in[0];
    int32_t points = (int32_t) val_in[1];
    int32_t fake_clusters = (int32_t) val_in[2];
    int32_t fake_points = (int32_t) val_in[3];
    Cluster clusters[MAX_CLUSTERS];

    // Number of clusters to read (num_clusters / 4)
    int32_t clusters_in = (num_clusters + fake_clusters) >> 2;

    // Read the coordinates of the clusters, assuming that the number of clusters is a multiple of 4
    for (size_t i = 0; i < clusters_in; i++)
    chess_loop_range(1,)
    chess_prepare_for_pipelining
    {
        val_in = readincr_v<8>(input);
        val_fl = aie::vector_cast<float>(val_in);

        for (size_t j = 0; j < 4; j++)
        {
            if (i * 4 + j < num_clusters)
            {
                clusters[i * 4 + j] = Cluster(val_fl[j * 2], val_fl[j * 2 + 1]);
                // std::cout << "Cluster " << i * 4 + j << ": (" << clusters[i * 4 + j].x << ", " << clusters[i * 4 + j].y << ")" << std::endl;
            }
        }
    }

    aie::vector<float, MAX_CLUSTERS> clusters_x, clusters_y;

    for (size_t i = 0; i < num_clusters; i++)
    chess_loop_range(1,)
    chess_prepare_for_pipelining
    {
        clusters_x[i] = clusters[i].x;
        clusters_y[i] = clusters[i].y;
    }

    aie::vector<float, MAX_CLUSTERS> distances;
    int32_t cluster_index = -1;
    int32_t num_points = points - fake_points;

    for (size_t i = 0; i < points; i += 4)
    chess_loop_range(1,)
    chess_prepare_for_pipelining
    {
        // Read the coordinates of the points, assuming that the number of points is a multiple of 4
        val_in = readincr_v<8>(input);
        val_fl = aie::vector_cast<float>(val_in);

        // Compute the algorithm for each of the 4 points
        for (size_t j = 0; j < 4; j++)
        chess_loop_range(4,)
        chess_prepare_for_pipelining
        {
            if (i + j < num_points)
            {
                Point point = Point(val_fl[j * 2], val_fl[j * 2 + 1]);
                // std::cout << "Point " << j << ": (" << point.x << ", " << point.y << ")" << std::endl;

                // Compute the euclidean distance between the point and all the clusters
                distances = euclidean_distance(clusters_x, clusters_y, num_clusters, point);

                // Assign the point to the closest cluster
                cluster_index = assignment_function(distances, num_clusters);
                clusters[cluster_index].addPoint(point);
            }
        }
    }

    aie::vector<float, 8> val_out;

    // Write the clusters to the output stream
    for (size_t i = 0; i < num_clusters; i++)
    chess_loop_range(1,)
    chess_prepare_for_pipelining
    {
        // Write the cluster coordinates to the output stream
        val_out[0] = clusters[i].x;
        val_out[1] = clusters[i].y;
        val_out[2] = 0;

        // Write the number of points in the cluster
        val_out[3] = clusters[i].numPoints;
        val_out[4] = 0;

        // Write the accumulated coordinates of the points in the cluster
        val_out[5] = clusters[i].x_accum;
        val_out[6] = clusters[i].y_accum;
        val_out[7] = 0;

        writeincr(output, val_out);
    }
}

// Compute the euclidean distance between a point and all the clusters
aie::vector<float, MAX_CLUSTERS> euclidean_distance(aie::vector<float, MAX_CLUSTERS> clusters_x, aie::vector<float, MAX_CLUSTERS> clusters_y, int32_t num_clusters, Point point)
{
    aie::vector<float, MAX_CLUSTERS> diff_x = aie::sub(clusters_x, point.x);
    aie::vector<float, MAX_CLUSTERS> diff_y = aie::sub(clusters_y, point.y);

    aie::accum<accfloat, MAX_CLUSTERS> dist_x = aie::mul_square(diff_x);
    aie::accum<accfloat, MAX_CLUSTERS> dist_y = aie::mul_square(diff_y);

    aie::vector<float, MAX_CLUSTERS> distances = aie::add(dist_x.to_vector<float>(), dist_y.to_vector<float>());
    return distances;
}

// Return the index of the cluster with the minimum distance from the point
int32_t assignment_function(aie::vector<float, MAX_CLUSTERS> distances, int32_t num_clusters)
{
    // Fill the remaining distances with INT32_MAX
    for (size_t i = num_clusters; i < MAX_CLUSTERS; i++)
    chess_prepare_for_pipelining
    {
        distances[i] = std::numeric_limits<float>::max();
    }

    float min_dist = aie::reduce_min(distances);

    for (size_t i = 0; i < num_clusters; i++)
    chess_loop_range(1,)
    chess_prepare_for_pipelining
    {
        if (distances[i] == min_dist)
        {
            return i;
        }
    }

    return -1;
}