#include <ap_int.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <ap_axi_sdata.h>
#include "../common/common.h"
#include "ap_fixed.h"

void compute(int32_t num_clusters, int32_t num_points, ap_uint<sizeof(float) * 8 * 8> *in, hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out)
{
	// Create a temporary variable to store the data (8 integers at a time = 4 points)
	ap_uint<sizeof(float) * 8 * 8> tmp;
	int32_t num_points_updated = num_points >> N_AIE_LOG;

	// Write the number of clusters and the number of points
	tmp.range(31, 0) = num_clusters;
	tmp.range(63, 32) = num_points_updated;
	tmp.range(255, 64) = 0;

	// Core Initialization
	out.write(tmp);

	// Write the clusters coordinates, assuming that their number is a multiple of 4
	int32_t cluster_read = num_clusters >> 2;
	for (size_t i = 0; i < cluster_read; i++)
	{
#pragma HLS pipeline II = 1
		out.write(in[i]);
	}

	// Write the points coordinates, assuming that their number is a multiple of 4
	int32_t point_read = num_points >> 2;
	for (size_t i = 0; i < point_read; i += N_AIE)
	{
#pragma HLS pipeline II = 1
		out.write(in[cluster_read + i + 0]);
	}
}

extern "C"
{
	void setup_aie(int32_t num_clusters, int32_t num_points, ap_uint<sizeof(float) * 8 * 8> *input, hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s)
	{
// PRAGMA for stream
#pragma HLS interface axis port = s

// PRAGMA for memory interation - AXI master-slave
#pragma HLS interface m_axi port = input depth = 100 offset = slave bundle = gmem0
#pragma HLS interface s_axilite port = input bundle = control

// PRAGMA for AXI-LITE : required to move params from host to PL
#pragma HLS interface s_axilite port = num_clusters bundle = control
#pragma HLS interface s_axilite port = num_points bundle = control
#pragma HLS interface s_axilite port = return bundle = control

// PRAGMA for DATAFLOW
#pragma DATAFLOW

		compute(num_clusters, num_points, input, s);
	}
}
