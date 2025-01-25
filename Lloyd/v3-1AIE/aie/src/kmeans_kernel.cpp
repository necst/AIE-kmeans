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
    int32_t num_points = (int32_t) val_in[1];
    Cluster clusters[MAX_CLUSTERS];

    // Number of clusters to read (num_clusters / 4)
    int32_t clusters_in = num_clusters >> 2;

    // Read the coordinates of the clusters, assuming that the number of clusters is a multiple of 4
    for (size_t i = 0; i < clusters_in; i++)
    {
        val_in = readincr_v<8>(input);
        val_fl = aie::vector_cast<float>(val_in);

        for (size_t j = 0; j < 4; j++)
        {
            clusters[i * 4 + j] = Cluster(val_fl[j * 2], val_fl[j * 2 + 1]);
            // std::cout << "Cluster " << i * 4 + j << ": (" << clusters[i * 4 + j].x << ", " << clusters[i * 4 + j].y << ")" << std::endl;
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

    for (size_t i = 0; i < num_points; i += 4)
    {
        // Read the coordinates of the points, assuming that the number of points is a multiple of 4
        val_in = readincr_v<8>(input);
        val_fl = aie::vector_cast<float>(val_in);
        size_t j = 0;

        // Compute the algorithm for each of the 4 points
        while (j < 4) {
            Point point = Point(val_fl[j * 2], val_fl[j * 2 + 1]);
            // std::cout << "Point " << j << ": (" << point.x << ", " << point.y << ")" << std::endl;

            // Compute the euclidean distance between the point and all the clusters
            distances = euclidean_distance(clusters_x, clusters_y, num_clusters, point);

            // Assign the point to the closest cluster
            cluster_index = assignment_function(distances, num_clusters);
            clusters[cluster_index].addPoint(point);
            // std::cout << "Assigned to cluster " << cluster_index << std::endl;

            j++;
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

/*         // Write the cluster coordinates to the output stream
        writeincr(output, clusters[i].x);
        writeincr(output, clusters[i].y);
        writeincr(output, 0);
        // std::cout << "Cluster " << i << ": (" << clusters[i].x << ", " << clusters[i].y << ")" << std::endl;

        // Write the number of points in the cluster
        writeincr(output, clusters[i].numPoints);
        writeincr(output, 0);
        // std::cout << "Number of points: " << clusters[i].numPoints << std::endl;

        // Write the accumulated coordinates of the points in the cluster
        writeincr(output, clusters[i].x_accum);
        writeincr(output, clusters[i].y_accum);
        writeincr(output, 0);
        // std::cout << "Accumulated coordinates: (" << clusters[i].x_accum << ", " << clusters[i].y_accum << ")" << std::endl;
 */    }
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
    // Fill the remaining distances with the maximum value
    for (size_t i = num_clusters; i < MAX_CLUSTERS; i++)
    {
        distances[i] = std::numeric_limits<float>::max();
    }

    float min_dist = aie::reduce_min(distances);

    for (size_t i = 0; i < num_clusters; i++)
    {
        if (distances[i] == min_dist)
        {
            return i;
        }
    }

    return -1;
}