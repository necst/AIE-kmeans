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
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out8,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out9,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out10,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out11,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out12,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out13,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out14,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out15,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out16,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out17,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out18,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out19,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out20,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out21,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out22,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out23,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out24,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out25,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out26,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out27,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out28,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out29,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out30,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out31,
	hls::stream<ap_uint<sizeof(float) * 8 * 8>> &out32
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
	out9.write(tmp);
	out10.write(tmp);
	out11.write(tmp);
	out12.write(tmp);
	out13.write(tmp);
	out14.write(tmp);
	out15.write(tmp);
	out16.write(tmp);
	out17.write(tmp);
	out18.write(tmp);
	out19.write(tmp);
	out20.write(tmp);
	out21.write(tmp);
	out22.write(tmp);
	out23.write(tmp);
	out24.write(tmp);
	out25.write(tmp);
	out26.write(tmp);
	out27.write(tmp);
	out28.write(tmp);
	out29.write(tmp);
	out30.write(tmp);
	out31.write(tmp);
	out32.write(tmp);

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
		out9.write(in[i]);
		out10.write(in[i]);
		out11.write(in[i]);
		out12.write(in[i]);
		out13.write(in[i]);
		out14.write(in[i]);
		out15.write(in[i]);
		out16.write(in[i]);
		out17.write(in[i]);
		out18.write(in[i]);
		out19.write(in[i]);
		out20.write(in[i]);
		out21.write(in[i]);
		out22.write(in[i]);
		out23.write(in[i]);
		out24.write(in[i]);
		out25.write(in[i]);
		out26.write(in[i]);
		out27.write(in[i]);
		out28.write(in[i]);
		out29.write(in[i]);
		out30.write(in[i]);
		out31.write(in[i]);
		out32.write(in[i]);
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
		out9.write(in[cluster_read + i + 8]);
		out10.write(in[cluster_read + i + 9]);
		out11.write(in[cluster_read + i + 10]);
		out12.write(in[cluster_read + i + 11]);
		out13.write(in[cluster_read + i + 12]);
		out14.write(in[cluster_read + i + 13]);
		out15.write(in[cluster_read + i + 14]);
		out16.write(in[cluster_read + i + 15]);
		out17.write(in[cluster_read + i + 16]);
		out18.write(in[cluster_read + i + 17]);
		out19.write(in[cluster_read + i + 18]);
		out20.write(in[cluster_read + i + 19]);
		out21.write(in[cluster_read + i + 20]);
		out22.write(in[cluster_read + i + 21]);
		out23.write(in[cluster_read + i + 22]);
		out24.write(in[cluster_read + i + 23]);
		out25.write(in[cluster_read + i + 24]);
		out26.write(in[cluster_read + i + 25]);
		out27.write(in[cluster_read + i + 26]);
		out28.write(in[cluster_read + i + 27]);
		out29.write(in[cluster_read + i + 28]);
		out30.write(in[cluster_read + i + 29]);
		out31.write(in[cluster_read + i + 30]);
		out32.write(in[cluster_read + i + 31]);
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
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_8,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_9,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_10,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_11,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_12,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_13,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_14,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_15,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_16,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_17,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_18,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_19,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_20,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_21,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_22,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_23,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_24,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_25,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_26,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_27,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_28,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_29,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_30,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_31,
		hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s_32
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
#pragma HLS interface axis port = s_17
#pragma HLS interface axis port = s_18
#pragma HLS interface axis port = s_19
#pragma HLS interface axis port = s_20
#pragma HLS interface axis port = s_21
#pragma HLS interface axis port = s_22
#pragma HLS interface axis port = s_23
#pragma HLS interface axis port = s_24
#pragma HLS interface axis port = s_25
#pragma HLS interface axis port = s_26
#pragma HLS interface axis port = s_27
#pragma HLS interface axis port = s_28
#pragma HLS interface axis port = s_29
#pragma HLS interface axis port = s_30
#pragma HLS interface axis port = s_31
#pragma HLS interface axis port = s_32

// PRAGMA for memory interation - AXI master-slave
#pragma HLS interface m_axi port = input depth = 100 offset = slave bundle = gmem0
#pragma HLS interface s_axilite port = input bundle = control

// PRAGMA for AXI-LITE : required to move params from host to PL
#pragma HLS interface s_axilite port = num_clusters bundle = control
#pragma HLS interface s_axilite port = num_points bundle = control
#pragma HLS interface s_axilite port = return bundle = control

// PRAGMA for DATAFLOW
#pragma DATAFLOW

		compute(num_clusters, num_points, input, s_1, s_2, s_3, s_4, s_5, s_6, s_7, s_8, s_9, s_10, s_11, s_12, s_13, s_14, s_15, s_16, s_17, s_18, s_19, s_20, s_21, s_22, s_23, s_24, s_25, s_26, s_27, s_28, s_29, s_30, s_31, s_32);
	}
}
