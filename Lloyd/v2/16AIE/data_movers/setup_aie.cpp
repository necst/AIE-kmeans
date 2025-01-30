#include <ap_int.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <ap_axi_sdata.h>
#include "../common/common.h"
#include "ap_fixed.h"

void compute(
	int32_t num_clusters, 
	int32_t num_points, 
	ap_uint<sizeof(float) * 8 * COORDS_IN> *in, 
	hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &out1, 
	hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &out2,
	hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &out3,
	hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &out4,
	hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &out5,
	hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &out6,
	hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &out7,
	hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &out8,
	hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &out9,
	hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &out10,
	hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &out11,
	hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &out12,
	hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &out13,
	hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &out14,
	hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &out15,
	hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &out16
	)
{
	// Create a temporary variable to store the data
	ap_uint<sizeof(float) * 8 * COORDS_IN> tmp;
	int32_t num_points_updated = num_points >> N_AIE_LOG;

	// Write the number of clusters and the number of points
	tmp.range(31, 0) = num_clusters;
	tmp.range(63, 32) = num_points_updated;
	tmp.range(255, 64) = 0;

	out1.write(tmp);
	out2.write(tmp);
	out3.write(tmp);
	out4.write(tmp);
	out5.write(tmp);
	out6.write(tmp);
	out7.write(tmp);
	out8.write(tmp);
	out9.write(tmp);
	out10.write(tmp);
	out11.write(tmp);
	out12.write(tmp);
	out13.write(tmp);
	out14.write(tmp);
	out15.write(tmp);
	out16.write(tmp);

	// Write the clusters coordinates, assuming that each vector can be completely filled
	int32_t cluster_read = num_clusters >> POINTS_LOG;
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
		out9.write(in[i]);
		out10.write(in[i]);
		out11.write(in[i]);
		out12.write(in[i]);
		out13.write(in[i]);
		out14.write(in[i]);
		out15.write(in[i]);
		out16.write(in[i]);
	}

	// Write the points coordinates, assuming that each vector can be completely filled
	int32_t point_read = num_points >> POINTS_LOG;
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
		out9.write(in[cluster_read + i + 8]);
		out10.write(in[cluster_read + i + 9]);
		out11.write(in[cluster_read + i + 10]);
		out12.write(in[cluster_read + i + 11]);
		out13.write(in[cluster_read + i + 12]);
		out14.write(in[cluster_read + i + 13]);
		out15.write(in[cluster_read + i + 14]);
		out16.write(in[cluster_read + i + 15]);
	}
}

extern "C"
{
	void setup_aie(
		int32_t num_clusters, 
		int32_t num_points, 
		ap_uint<sizeof(float) * 8 * COORDS_IN> *input, 
		hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &s_1, 
		hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &s_2,
		hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &s_3,
		hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &s_4,
		hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &s_5,
		hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &s_6,
		hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &s_7,
		hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &s_8,
		hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &s_9,
		hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &s_10,
		hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &s_11,
		hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &s_12,
		hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &s_13,
		hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &s_14,
		hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &s_15,
		hls::stream<ap_uint<sizeof(float) * 8 * COORDS_IN>> &s_16
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
#pragma HLS interface axis port = s_9
#pragma HLS interface axis port = s_10
#pragma HLS interface axis port = s_11
#pragma HLS interface axis port = s_12
#pragma HLS interface axis port = s_13
#pragma HLS interface axis port = s_14
#pragma HLS interface axis port = s_15
#pragma HLS interface axis port = s_16

// PRAGMA for memory interation - AXI master-slave
#pragma HLS interface m_axi port = input depth = 100 offset = slave bundle = gmem0
#pragma HLS interface s_axilite port = input bundle = control

// PRAGMA for AXI-LITE : required to move params from host to PL
#pragma HLS interface s_axilite port = num_clusters bundle = control
#pragma HLS interface s_axilite port = num_points bundle = control
#pragma HLS interface s_axilite port = return bundle = control

// PRAGMA for DATAFLOW
#pragma DATAFLOW
		compute(num_clusters, num_points, input, s_1, s_2 , s_3, s_4, s_5, s_6, s_7, s_8, s_9, s_10, s_11, s_12, s_13, s_14, s_15, s_16);
	}
}
