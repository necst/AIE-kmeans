#include <ap_int.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <ap_axi_sdata.h>
#include "../common/common.h"
#include "ap_fixed.h"

// Move the data from the host to the AIE
void compute(
	int32_t num_clusters, 
	int32_t num_points, 
	ap_uint<sizeof(float) * 8 * COORDS_IN> *input, 
	hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &s
	)
{
	// Create a temporary variable to store the data
	ap_uint<sizeof(float) * 8 * COORDS_IN> tmp;

	// Write the number of clusters and the number of points
	tmp.range(31, 0) = num_clusters;
	tmp.range(63, 32) = num_points;
	tmp.range(255, 64) = 0;
	s.write(tmp);

	// Write the clusters coordinates, assuming that each vector can be completely filled
	int32_t cluster_read = num_clusters >> POINTS_LOG;
	for (int32_t i = 0; i < cluster_read; i++)
	{
#pragma HLS pipeline II = 1
		s.write(input[i]);
	}

	// Write the points coordinates, assuming that each vector can be completely filled
	int32_t point_read = num_points >> POINTS_LOG;
	for (int32_t i = 0; i < point_read; i++)
	{
#pragma HLS pipeline II = 1
		s.write(input[cluster_read + i]);
	}
}

extern "C"
{
	void setup_aie(
		int32_t num_clusters, 
		int32_t num_points, 
		ap_uint<sizeof(float) * 8 * COORDS_IN> *input, 
		hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &s
		)
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
