#include <ap_int.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <ap_axi_sdata.h>
#include "../common/common.h"

void compute(
    hls::stream<float> &in_1, 
    hls::stream<float> &in_2,
    hls::stream<float> &in_3,
    hls::stream<float> &in_4,
    hls::stream<float> &in_5,
    hls::stream<float> &in_6,
    hls::stream<float> &in_7,
    hls::stream<float> &in_8,
    hls::stream<float> &in_9,
    hls::stream<float> &in_10,
    hls::stream<float> &in_11,
    hls::stream<float> &in_12,
    hls::stream<float> &in_13,
    hls::stream<float> &in_14,
    hls::stream<float> &in_15,
    hls::stream<float> &in_16,
    hls::stream<float> &in_17,
    hls::stream<float> &in_18,
    hls::stream<float> &in_19,
    hls::stream<float> &in_20,
    hls::stream<float> &in_21,
    hls::stream<float> &in_22,
    hls::stream<float> &in_23,
    hls::stream<float> &in_24,
    hls::stream<float> &in_25,
    hls::stream<float> &in_26,
    hls::stream<float> &in_27,
    hls::stream<float> &in_28,
    hls::stream<float> &in_29,
    hls::stream<float> &in_30,
    hls::stream<float> &in_31,
    hls::stream<float> &in_32,
    float *out, 
    int32_t num_clusters
    )
{
    float discard;

    // Merge the clusters from the AIEs and update the cluster coordinates
    for (size_t i = 0; i < num_clusters; i++)
    {
#pragma HLS pipeline II = 1

        // Read the cluster coordinates from the first AIE
        float x = in_1.read();
        discard = in_2.read() + in_3.read() + in_4.read() + in_5.read() + in_6.read() + in_7.read() + in_8.read() + in_9.read() + in_10.read() + in_11.read() + in_12.read() + in_13.read() + in_14.read() + in_15.read() + in_16.read() + in_17.read() + in_18.read() + in_19.read() + in_20.read() + in_21.read() + in_22.read() + in_23.read() + in_24.read() + in_25.read() + in_26.read() + in_27.read() + in_28.read() + in_29.read() + in_30.read() + in_31.read() + in_32.read();
        float y = in_1.read();
        discard = in_2.read() + in_3.read() + in_4.read() + in_5.read() + in_6.read() + in_7.read() + in_8.read() + in_9.read() + in_10.read() + in_11.read() + in_12.read() + in_13.read() + in_14.read() + in_15.read() + in_16.read() + in_17.read() + in_18.read() + in_19.read() + in_20.read() + in_21.read() + in_22.read() + in_23.read() + in_24.read() + in_25.read() + in_26.read() + in_27.read() + in_28.read() + in_29.read() + in_30.read() + in_31.read() + in_32.read();
        discard = in_1.read() + in_2.read() + in_3.read() + in_4.read() + in_5.read() + in_6.read() + in_7.read() + in_8.read() + in_9.read() + in_10.read() + in_11.read() + in_12.read() + in_13.read() + in_14.read() + in_15.read() + in_16.read() + in_17.read() + in_18.read() + in_19.read() + in_20.read() + in_21.read() + in_22.read() + in_23.read() + in_24.read() + in_25.read() + in_26.read() + in_27.read() + in_28.read() + in_29.read() + in_30.read() + in_31.read() + in_32.read();

        // Sum the number of points assigned to the cluster
        int32_t num_points = (int32_t) (in_1.read() + in_2.read() + in_3.read() + in_4.read() + in_5.read() + in_6.read() + in_7.read() + in_8.read() + in_9.read() + in_10.read() + in_11.read() + in_12.read() + in_13.read() + in_14.read() + in_15.read() + in_16.read() + in_17.read() + in_18.read() + in_19.read() + in_20.read() + in_21.read() + in_22.read() + in_23.read() + in_24.read() + in_25.read() + in_26.read() + in_27.read() + in_28.read() + in_29.read() + in_30.read() + in_31.read() + in_32.read() - (N_AIE - 1));
        discard = in_1.read() + in_2.read() + in_3.read() + in_4.read() + in_5.read() + in_6.read() + in_7.read() + in_8.read() + in_9.read() + in_10.read() + in_11.read() + in_12.read() + in_13.read() + in_14.read() + in_15.read() + in_16.read() + in_17.read() + in_18.read() + in_19.read() + in_20.read() + in_21.read() + in_22.read() + in_23.read() + in_24.read() + in_25.read() + in_26.read() + in_27.read() + in_28.read() + in_29.read() + in_30.read() + in_31.read() + in_32.read();

        // Sum the accumulated coordinates of the points in the cluster
        float x_accum = in_1.read() + in_2.read() + in_3.read() + in_4.read() + in_5.read() + in_6.read() + in_7.read() + in_8.read() + in_9.read() + in_10.read() + in_11.read() + in_12.read() + in_13.read() + in_14.read() + in_15.read() + in_16.read() + in_17.read() + in_18.read() + in_19.read() + in_20.read() + in_21.read() + in_22.read() + in_23.read() + in_24.read() + in_25.read() + in_26.read() + in_27.read() + in_28.read() + in_29.read() + in_30.read() + in_31.read() + in_32.read();
        float y_accum = in_1.read() + in_2.read() + in_3.read() + in_4.read() + in_5.read() + in_6.read() + in_7.read() + in_8.read() + in_9.read() + in_10.read() + in_11.read() + in_12.read() + in_13.read() + in_14.read() + in_15.read() + in_16.read() + in_17.read() + in_18.read() + in_19.read() + in_20.read() + in_21.read() + in_22.read() + in_23.read() + in_24.read() + in_25.read() + in_26.read() + in_27.read() + in_28.read() + in_29.read() + in_30.read() + in_31.read() + in_32.read();
        discard = in_1.read() + in_2.read() + in_3.read() + in_4.read() + in_5.read() + in_6.read() + in_7.read() + in_8.read() + in_9.read() + in_10.read() + in_11.read() + in_12.read() + in_13.read() + in_14.read() + in_15.read() + in_16.read() + in_17.read() + in_18.read() + in_19.read() + in_20.read() + in_21.read() + in_22.read() + in_23.read() + in_24.read() + in_25.read() + in_26.read() + in_27.read() + in_28.read() + in_29.read() + in_30.read() + in_31.read() + in_32.read();

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
        hls::stream<float> &input_3,
        hls::stream<float> &input_4,
        hls::stream<float> &input_5,
        hls::stream<float> &input_6,
        hls::stream<float> &input_7,
        hls::stream<float> &input_8,
        hls::stream<float> &input_9,
        hls::stream<float> &input_10,
        hls::stream<float> &input_11,
        hls::stream<float> &input_12,
        hls::stream<float> &input_13,
        hls::stream<float> &input_14,
        hls::stream<float> &input_15,
        hls::stream<float> &input_16,
        hls::stream<float> &input_17,
        hls::stream<float> &input_18,
        hls::stream<float> &input_19,
        hls::stream<float> &input_20,
        hls::stream<float> &input_21,
        hls::stream<float> &input_22,
        hls::stream<float> &input_23,
        hls::stream<float> &input_24,
        hls::stream<float> &input_25,
        hls::stream<float> &input_26,
        hls::stream<float> &input_27,
        hls::stream<float> &input_28,
        hls::stream<float> &input_29,
        hls::stream<float> &input_30,
        hls::stream<float> &input_31,
        hls::stream<float> &input_32, 
        float *output, 
        int32_t num_clusters
        )
    {
// PRAGMA for stream
#pragma HLS interface axis port = input_1
#pragma HLS interface axis port = input_2
#pragma HLS interface axis port = input_3
#pragma HLS interface axis port = input_4
#pragma HLS interface axis port = input_5
#pragma HLS interface axis port = input_6
#pragma HLS interface axis port = input_7
#pragma HLS interface axis port = input_8
#pragma HLS interface axis port = input_9
#pragma HLS interface axis port = input_10
#pragma HLS interface axis port = input_11
#pragma HLS interface axis port = input_12
#pragma HLS interface axis port = input_13
#pragma HLS interface axis port = input_14
#pragma HLS interface axis port = input_15
#pragma HLS interface axis port = input_16
#pragma HLS interface axis port = input_17
#pragma HLS interface axis port = input_18
#pragma HLS interface axis port = input_19
#pragma HLS interface axis port = input_20
#pragma HLS interface axis port = input_21
#pragma HLS interface axis port = input_22
#pragma HLS interface axis port = input_23
#pragma HLS interface axis port = input_24
#pragma HLS interface axis port = input_25
#pragma HLS interface axis port = input_26
#pragma HLS interface axis port = input_27
#pragma HLS interface axis port = input_28
#pragma HLS interface axis port = input_29
#pragma HLS interface axis port = input_30
#pragma HLS interface axis port = input_31
#pragma HLS interface axis port = input_32

// PRAGMA for memory interation - AXI master-slave
#pragma HLS INTERFACE m_axi port = output depth = 100 offset = slave bundle = gmem1
#pragma HLS INTERFACE s_axilite port = output bundle = control

// PRAGMA for AXI-LITE : required to move params from host to PL
#pragma HLS interface s_axilite port = num_clusters bundle = control
#pragma HLS interface s_axilite port = return bundle = control

// PRAGMA for DATAFLOW
#pragma DATAFLOW
        compute(input_1, input_2, input_3, input_4, input_5, input_6, input_7, input_8, input_9, input_10, input_11, input_12, input_13, input_14, input_15, input_16, input_17, input_18, input_19, input_20, input_21, input_22, input_23, input_24, input_25, input_26, input_27, input_28, input_29, input_30, input_31, input_32, output, num_clusters);
    }
}
