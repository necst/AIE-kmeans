#include "kmeans_kernel.h"
#include "common.h"
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "aie_api/utils.hpp"

aie::vector<float, MAX_CLUSTERS> euclidean_distance(aie::vector<float, MAX_CLUSTERS> clusters_x, aie::vector<float, MAX_CLUSTERS> clusters_y, int32_t num_clusters, Point point);
int32_t nearest_cluster(aie::vector<float, MAX_CLUSTERS> distances, int32_t num_clusters);

// Lloyd's implementation of K-Means algorithm
void kmeans_function(input_stream<int32_t> *restrict input, output_stream<float> *restrict output)
{
    aie::vector<int32_t, COORDS_IN> val_in = readincr_v<COORDS_IN>(input);
    aie::vector<float, COORDS_IN> val_fl;
    Cluster clusters[MAX_CLUSTERS];

    // Read the number of clusters and points
    int32_t num_clusters = (int32_t)c[0];
    int32_t num_points = (int32_t)val_in[1];

    // Number of cycles to read all clusters
    int32_t clusters_in = num_clusters >> POINTS_LOG;

    // Read the coordinates of clusters, assuming that each vector read is completely filled
    for (size_t i = 0; i < clusters_in; i++)
    {
        val_in = readincr_v<COORDS_IN>(input);
        val_fl = aie::vector_cast<float>(val_in);

        for (size_t j = 0; j < POINTS; j++)
        {
            clusters[i * POINTS + j] = Cluster(val_fl[j * 2], val_fl[j * 2 + 1]);
        }
    }

    aie::vector<float, MAX_CLUSTERS> clusters_x, clusters_y;

    for (size_t i = 0; i < num_clusters; i++)
    {
        clusters_x[i] = clusters[i].x;
        clusters_y[i] = clusters[i].y;
    }

    aie::vector<float, MAX_CLUSTERS> distances;
    int32_t cluster_index = -1;

    for (size_t i = 0; i < num_points; i += POINTS)
    {
        // Read the coordinates of the points, assuming that each vector read is completely filled
        val_in = readincr_v<COORDS_IN>(input);
        val_fl = aie::vector_cast<float>(val_in);

        // Compute the algorithm for each point
        for (size_t j = 0; j < POINTS; j++)
        {
            Point point = Point(val_fl[j * 2], val_fl[j * 2 + 1]);

            // Compute the euclidean distance between the point and all the clusters
            distances = euclidean_distance(clusters_x, clusters_y, num_clusters, point);

            // Assign the point to the closest cluster
            cluster_index = assignment_function(distances, num_clusters);
            clusters[cluster_index].addPoint(point);
        }
    }

    aie::vector<float, 8> val_out;

    // Write the clusters to the output stream
    for (size_t i = 0; i < num_clusters; i++)
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
    // Compute the differences between the point and all the clusters
    aie::vector<float, MAX_CLUSTERS> diff_x = aie::sub(clusters_x, point.x);
    aie::vector<float, MAX_CLUSTERS> diff_y = aie::sub(clusters_y, point.y);

    // Compute the square of the differences
    aie::accum<accfloat, MAX_CLUSTERS> dist_x = aie::mul_square(diff_x);
    aie::accum<accfloat, MAX_CLUSTERS> dist_y = aie::mul_square(diff_y);

    // Compute the sum of the squares
    aie::vector<float, MAX_CLUSTERS> distances = aie::add(dist_x.to_vector<float>(), dist_y.to_vector<float>());
    return distances;
}

// Return the index of the cluster with the minimum distance from the point
int32_t assignment_function(aie::vector<float, MAX_CLUSTERS> distances, int32_t num_clusters)
{
    // Fill the remaining distances with the maximum value
    for (size_t i = num_clusters; i < MAX_CLUSTERS; i++)
    {
        distances[i] = std::numeric_limits<float>::max();
    }

    // Find the minimum distance within the distances
    float min_dist = aie::reduce_min(distances);

    // Return the index of the cluster with the minimum distance
    {
        if (distances[i] == min_dist)
        {
            return i;
        }
    }

    return -1;
}