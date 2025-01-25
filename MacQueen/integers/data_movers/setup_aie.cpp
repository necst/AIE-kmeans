#include <ap_int.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <ap_axi_sdata.h>
#include "../common/common.h"

void compute(int32_t num_clusters, int32_t num_points, ap_int<sizeof(int32_t) * 8 * 8> *input, hls::stream<ap_int<sizeof(int32_t) * 8 * 8>> &s)
{
	// Create a temporary variable to store the data (8 integers at a time = 4 points)
	ap_int<sizeof(int32_t) * 8 * 8> tmp;

	// Write the number of clusters and the number of points
	tmp.range(31, 0) = num_clusters;
	tmp.range(63, 32) = num_points;
	tmp.range(255, 64) = 0;
	s.write(tmp);

	// Write the clusters and points coordinates, assuming that their number is a multiple of 4
	int32_t read_size = (num_clusters + num_points) >> 2;
	for (int32_t i = 0; i < read_size; i++)
	{
#pragma HLS pipeline II = 1
		s.write(input[i]);
	}
}

extern "C"
{
	void setup_aie(int32_t num_clusters, int32_t num_points, ap_int<sizeof(int32_t) * 8 * 8> *input, hls::stream<ap_int<sizeof(int32_t) * 8 * 8>> &s)
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