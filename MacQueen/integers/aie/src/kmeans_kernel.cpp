#include "kmeans_kernel.h"
#include "common.h"
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "aie_api/utils.hpp"

aie::vector<int32_t, MAX_CLUSTERS> euclidean_distance(aie::vector<int32_t, MAX_CLUSTERS> clusters_x, aie::vector<int32_t, MAX_CLUSTERS> clusters_y, int32_t num_clusters, Point point);
int32_t nearest_cluster(aie::vector<int32_t, MAX_CLUSTERS> distances, int32_t num_clusters);

// MacQueen's implementation of K-Means algorithm
void kmeans_function(input_stream<int32_t> *restrict input, output_stream<int32_t> *restrict output)
{
    aie::vector<int32_t, COORDS_IN> val_in = readincr_v<COORDS_IN>(input);
    Cluster clusters[MAX_CLUSTERS];

    // Read the number of clusters and points
    int32_t num_clusters = val_in[0];
    int32_t num_points = val_in[1];

    // Number of cycles to read all clusters
    int32_t clusters_in = num_clusters >> POINTS_LOG;

    // Read the coordinates of clusters, assuming that each vector read is completely filled
    for (size_t i = 0; i < clusters_in; i++)
    {
        val_in = readincr_v<COORDS_IN>(input);

        for (size_t j = 0; j < POINTS; j++)
        {
            clusters[i * POINTS + j] = Cluster(val_in[j * 2], val_in[j * 2 + 1]);
        }
    }

    aie::vector<int32_t, MAX_CLUSTERS> clusters_x, clusters_y;

    for (size_t i = 0; i < num_clusters; i++)
    {
        clusters_x[i] = clusters[i].x;
        clusters_y[i] = clusters[i].y;
    }

    aie::vector<int32_t, MAX_CLUSTERS> distances;
    int32_t cluster_index = -1;

    for (size_t i = 0; i < num_points; i += POINTS)
    {
        // Read the coordinates of the points, assuming that each vector read is completely filled
        val_in = readincr_v<COORDS_IN>(input);

        // Compute the algorithm for each point
        for (size_t j = 0; j < POINTS; j++)
        {
            Point point = Point(val_in[j * 2], val_in[j * 2 + 1]);

            // Compute the euclidean distance between the point and all the clusters
            distances = euclidean_distance(clusters_x, clusters_y, num_clusters, point);

            // Get the index of the nearest cluster
            cluster_index = nearest_cluster(distances, num_clusters);

            // Assign the point to the closest cluster and update the cluster coordinates
            clusters[cluster_index].addPoint(point);
        }
    }

    // Write the coordinates of the clusters in the output stream
    for (size_t i = 0; i < num_clusters; i++)
    {
        writeincr(output, clusters[i].x);
        writeincr(output, clusters[i].y);
    }
}

// Compute the euclidean distance between a point and all the clusters
aie::vector<int32_t, MAX_CLUSTERS> euclidean_distance(aie::vector<int32_t, MAX_CLUSTERS> clusters_x, aie::vector<int32_t, MAX_CLUSTERS> clusters_y, int32_t num_clusters, Point point)
{
    // Compute the differences between the point and the clusters
    aie::vector<int32_t, MAX_CLUSTERS> diff_x = aie::sub(clusters_x, point.x);
    aie::vector<int32_t, MAX_CLUSTERS> diff_y = aie::sub(clusters_y, point.y);

    // Compute the square of the differences
    aie::accum<acc64, MAX_CLUSTERS> dist_x = aie::mul_square(diff_x);
    aie::accum<acc64, MAX_CLUSTERS> dist_y = aie::mul_square(diff_y);

    // Compute the sum of the squares
    aie::vector<int32_t, MAX_CLUSTERS> distances = aie::add(dist_x.to_vector<int32_t>(), dist_y.to_vector<int32_t>());
    return distances;
}

// Return the index of the cluster with the minimum distance from the point
int32_t nearest_cluster(aie::vector<int32_t, MAX_CLUSTERS> distances, int32_t num_clusters)
{
    // Fill the remaining distances with INT32_MAX
    for (size_t i = num_clusters; i < MAX_CLUSTERS; i++)
    {
        distances[i] = __INT32_MAX__;
    }

    // Find the minimum distance within the distances
    int32_t min_dist = aie::reduce_min(distances);

    // Return the index of the cluster with the minimum distance    
    for (size_t i = 0; i < num_clusters; i++)
    {
        if (distances[i] == min_dist)
        {
            return i;
        }
    }

    return -1;
}