#pragma once
#include <adf.h>
#include "kmeans_kernel.h"
#include "common.h"

using namespace adf;

class my_graph : public graph
{
private:
	// ------kernel declaration------
	kernel kmeans_kernels[N_AIE];

public:
	// ------Input and Output PLIO declaration------
	input_plio in_kmeans[N_AIE];
	output_plio out_kmeans[N_AIE];

	my_graph()
	{
		for (int i = 0; i < N_AIE; i++)
		{
			// ------kernel creation------
			kmeans_kernels[i] = kernel::create(kmeans_function);

			// ------Input and Output PLIO creation------
			// arguments: name of the port (for the block design), type of the PLIO that will be read/written, path to the file that will be read/written (for the simulation)
			std::string idx = std::to_string(i + 1);
			in_kmeans[i] = input_plio::create("in_plio_kmeans_" + idx, plio_32_bits, "data/in_plio_source_" + idx + ".txt");
			out_kmeans[i] = output_plio::create("out_plio_kmeans_" + idx, plio_32_bits, "data/out_plio_sink_" + idx + ".txt");

			// ------kernel connection------
			connect<stream>(in_kmeans[i].out[0], kmeans_kernels[i].in[0]);
			connect<stream>(kmeans_kernels[i].out[0], out_kmeans[i].in[0]);

			// set kernel source and headers
			source(kmeans_kernels[i]) = "src/kmeans_kernel.cpp";
			headers(kmeans_kernels[i]) = {"src/kmeans_kernel.h", "../common/common.h"};
			runtime<ratio>(kmeans_kernels[i]) = 0.9;
		}
	};
};
