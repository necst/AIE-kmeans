#include <ap_int.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <ap_axi_sdata.h>
#include "../common/common.h"
#include <random> // Add this line for mt19937 and uniform distributions

// Helper function to extract float from ap_uint
/* float extract_float(const ap_uint<32> &bits)
{
    union
    {
        int32_t i;
        float f;
    } converter;
    converter.i = bits.to_int();
    return converter.f;
} */

void compute(hls::stream</* ap_uint<sizeof(float) * 8 * 8> */float> &in, float *out, int32_t num_clusters)
{
    float discard;

    // Merge the clusters from the two AIEs and update the cluster coordinates
    for (size_t i = 0; i < num_clusters; i++)
    {
#pragma HLS pipeline II = 1

        // Read the packed data from the input streams
        // ap_uint<sizeof(float) * 8 * 8> data = in.read();

        // Read the cluster coordinates from the first AIE
        // float x = extract_float(data.range(31, 0));
        // float y = extract_float(data.range(63, 32));
        float x = in.read();
        float y = in.read();
        discard = in.read();

        // Read the number of points assigned to the cluster 
        // int32_t num_points = (int32_t) data.range(127, 96);
        int32_t num_points = (int32_t) in.read();
        discard = in.read();

        // Read the accumulated coordinates of the points in the cluster
        // float x_accum = extract_float(data.range(191, 160));
        // float y_accum = extract_float(data.range(223, 192));
        float x_accum = in.read();
        float y_accum = in.read();
        discard = in.read();

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
    void sink_from_aie(hls::stream</* ap_uint<sizeof(float) * 8 * 8> */float> &input, float *output, int32_t num_clusters)
    {
// PRAGMA for stream
#pragma HLS interface axis port = input

// PRAGMA for memory interation - AXI master-slave
#pragma HLS INTERFACE m_axi port = output depth = 100 offset = slave bundle = gmem1
#pragma HLS INTERFACE s_axilite port = output bundle = control

// PRAGMA for AXI-LITE : required to move params from host to PL
#pragma HLS interface s_axilite port = num_clusters bundle = control
#pragma HLS interface s_axilite port = return bundle = control

// PRAGMA for DATAFLOW
#pragma DATAFLOW

        compute(input, output, num_clusters);
    }
}
