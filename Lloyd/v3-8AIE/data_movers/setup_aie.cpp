#include <ap_int.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <ap_axi_sdata.h>
#include "../common/common.h"
#include "ap_fixed.h"

void compute(
	int32_t num_clusters, 
	int32_t num_points, 
	ap_uint<sizeof(float) * 8 * 8> *in, 
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out1, 
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out2,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out3,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out4,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out5,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out6,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out7,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out8
	)
{
	// Create a temporary variable to store the data (8 integers at a time = 4 points)
	ap_uint<sizeof(float) * 8 * 8> tmp;
	int32_t num_points_updated = num_points >> N_AIE_LOG;

	// Write the number of clusters and the number of points
	tmp.range(31, 0) = num_clusters;
	tmp.range(63, 32) = num_points_updated;
	tmp.range(255, 64) = 0;

	// Core Initialization
	out1.write(tmp);
	out2.write(tmp);
	out3.write(tmp);
	out4.write(tmp);
	out5.write(tmp);
	out6.write(tmp);
	out7.write(tmp);
	out8.write(tmp);

	// Write the clusters coordinates, assuming that their number is a multiple of 4
	int32_t cluster_read = num_clusters >> 2;
	for (int32_t i = 0; i < cluster_read; i++)
	{
#pragma HLS pipeline II = 1
		out1.write(in[i]);
		out2.write(in[i]);
		out3.write(in[i]);
		out4.write(in[i]);
		out5.write(in[i]);
		out6.write(in[i]);
		out7.write(in[i]);
		out8.write(in[i]);
	}

	// Write the points coordinates, assuming that their number is a multiple of 8 (4 points in each cluster)
	int32_t point_read = num_points >> 2;
	for (int32_t i = 0; i < point_read; i += N_AIE)
	{
#pragma HLS pipeline II = 1
		out1.write(in[cluster_read + i + 0]);
		out2.write(in[cluster_read + i + 1]);
		out3.write(in[cluster_read + i + 2]);
		out4.write(in[cluster_read + i + 3]);
		out5.write(in[cluster_read + i + 4]);
		out6.write(in[cluster_read + i + 5]);
		out7.write(in[cluster_read + i + 6]);
		out8.write(in[cluster_read + i + 7]);
	}
}

extern "C"
{
	void setup_aie(
		int32_t num_clusters, 
		int32_t num_points, 
		ap_uint<sizeof(float) * 8 * 8> *input, 
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_1, 
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_2,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_3,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_4,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_5,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_6,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_7,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_8
		)
	{
// PRAGMA for stream
#pragma HLS interface axis port = s_1
#pragma HLS interface axis port = s_2
#pragma HLS interface axis port = s_3
#pragma HLS interface axis port = s_4
#pragma HLS interface axis port = s_5
#pragma HLS interface axis port = s_6
#pragma HLS interface axis port = s_7
#pragma HLS interface axis port = s_8

// PRAGMA for memory interation - AXI master-slave
#pragma HLS interface m_axi port = input depth = 100 offset = slave bundle = gmem0
#pragma HLS interface s_axilite port = input bundle = control

// PRAGMA for AXI-LITE : required to move params from host to PL
#pragma HLS interface s_axilite port = num_clusters bundle = control
#pragma HLS interface s_axilite port = num_points bundle = control
#pragma HLS interface s_axilite port = return bundle = control

// PRAGMA for DATAFLOW
#pragma DATAFLOW

		compute(num_clusters, num_points, input, s_1, s_2, s_3, s_4, s_5, s_6, s_7, s_8);
	}
}