#include <ap_int.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <ap_axi_sdata.h>
#include "../common/common.h"

void compute(
    hls::stream<float> &in_1, 
    hls::stream<float> &in_2, 
    float *out, 
    int32_t num_clusters)
{
    float discard;

    // Merge the clusters from the AIEs and update the cluster coordinates
    for (size_t i = 0; i < num_clusters; i++)
    {
#pragma HLS pipeline II = 1

        // Read the cluster coordinates from the first AIE
        // All the streams should contain the same data for x and y coordinates
        float x = in_1.read();
        discard = in_2.read();

        float y = in_1.read();
        discard = in_2.read();
        discard = in_1.read() + in_2.read();

        // Sum the number of points assigned to the cluster
        int32_t num_points = (int32_t) in_1.read() + (int32_t) in_2.read() - 1;
        discard = in_1.read() + in_2.read();

        // Sum the accumulated coordinates of the points in the cluster
        float x_accum = in_1.read() + in_2.read();
        float y_accum = in_1.read() + in_2.read();
        discard = in_1.read() + in_2.read();

        // Update the cluster coordinates
        float cluster_x = (x + x_accum) / num_points;
        float cluster_y = (y + y_accum) / num_points;

        // Write the updated cluster coordinates to the output stream
        out[i * 2 + 0] = cluster_x;
        out[i * 2 + 1] = cluster_y;
    }
}

extern "C"
{
    void sink_from_aie(
        hls::stream<float> &input_1, 
        hls::stream<float> &input_2, 
        float *output, 
        int32_t num_clusters)
    {
// PRAGMA for stream
#pragma HLS interface axis port = input_1
#pragma HLS interface axis port = input_2

// PRAGMA for memory interation - AXI master-slave
#pragma HLS INTERFACE m_axi port = output depth = 100 offset = slave bundle = gmem1
#pragma HLS INTERFACE s_axilite port = output bundle = control

// PRAGMA for AXI-LITE : required to move params from host to PL
#pragma HLS interface s_axilite port = num_clusters bundle = control
#pragma HLS interface s_axilite port = return bundle = control

// PRAGMA for DATAFLOW
#pragma DATAFLOW
        compute(input_1, input_2, output, num_clusters);
    }
}
