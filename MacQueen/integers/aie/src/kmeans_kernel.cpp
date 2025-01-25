#include "kmeans_kernel.h"
#include "common.h"
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "aie_api/utils.hpp"

aie::vector<int32_t, MAX_CLUSTERS> euclidean_distance(Cluster *clusters, int32_t num_clusters, Point point);
int32_t assignment_function(aie::vector<int32_t, MAX_CLUSTERS> distances, int32_t num_clusters);

void kmeans_function(input_stream<int32_t> *restrict input, output_stream<int32_t> *restrict output)
{
    aie::vector<int32_t, 8> val_in = readincr_v<8>(input);
    int32_t num_clusters = val_in[0];
    int32_t num_points = val_in[1];

    Cluster clusters[MAX_CLUSTERS];

    // Number of clusters to read (num_clusters / 4)
    int32_t clusters_in = num_clusters >> 2;

    // Read the coordinates of the clusters, assuming that the number of clusters is a multiple of 4
    for (size_t i = 0; i < clusters_in; i++)
    {
        val_in = readincr_v<8>(input);

        for (size_t j = 0; j < 4; j++)
        {
            clusters[i * 4 + j] = Cluster(val_in[j * 2], val_in[j * 2 + 1]);
        }
    }

    aie::vector<int32_t, MAX_CLUSTERS> distances;
    int32_t cluster_index = -1;

    for (size_t i = 0; i < num_points; i += 4)
    {
        // Read the coordinates of the points, assuming that the number of points is a multiple of 4
        val_in = readincr_v<8>(input);
        size_t j = 0;

        // Compute the algorithm for each point
        while (j < 4) {
            Point point = Point(val_in[j * 2], val_in[j * 2 + 1]);

            // Compute the euclidean distance between the point and all the clusters
            distances = euclidean_distance(clusters, num_clusters, point);

            // Assign the point to the closest cluster and update the cluster coordinates
            cluster_index = assignment_function(distances, num_clusters);
            clusters[cluster_index].addPoint(point);

            j++;
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
aie::vector<int32_t, MAX_CLUSTERS> euclidean_distance(Cluster *clusters, int32_t num_clusters, Point point)
{
    aie::vector<int32_t, MAX_CLUSTERS> clusters_x, clusters_y;

    for (size_t i = 0; i < num_clusters; i++)
    {
        clusters_x[i] = clusters[i].x;
        clusters_y[i] = clusters[i].y;
    }

    aie::vector<int32_t, MAX_CLUSTERS> diff_x = aie::sub(clusters_x, point.x);
    aie::vector<int32_t, MAX_CLUSTERS> diff_y = aie::sub(clusters_y, point.y);

    aie::accum<acc64, MAX_CLUSTERS> dist_x = aie::mul(diff_x, diff_x);
    aie::accum<acc64, MAX_CLUSTERS> dist_y = aie::mul(diff_y, diff_y);

    aie::vector<int32_t, MAX_CLUSTERS> distances = aie::add(dist_x.to_vector<int32_t>(), dist_y.to_vector<int32_t>());
    return distances;
}

// Return the index of the cluster with the minimum distance from the point
int32_t assignment_function(aie::vector<int32_t, MAX_CLUSTERS> distances, int32_t num_clusters)
{
    // Fill the remaining distances with INT32_MAX
    for (size_t i = num_clusters; i < MAX_CLUSTERS; i++)
    {
        distances[i] = __INT32_MAX__;
    }

    int32_t min_dist = aie::reduce_min(distances);

    for (size_t i = 0; i < num_clusters; i++)
    {
        if (distances[i] == min_dist)
        {
            return i;
        }
    }

    return -1;
}