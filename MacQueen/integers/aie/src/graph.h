#pragma once
#include <adf.h>
#include "kmeans_kernel.h"

using namespace adf;

class my_graph : public graph
{
	private:
		// ------kernel declaration------
		kernel kmeans_kernel;

	public:
		// ------Input and Output PLIO declaration------
		input_plio in_kmeans;
		output_plio out_kmeans;

		my_graph()
		{
			// ------kernel creation------
			kmeans_kernel = kernel::create(kmeans_function);

			// ------Input and Output PLIO creation------
			// arguments: name of the port (for the block design), type of the PLIO that will be read/written, path to the file that will be read/written (for the simulation)
			in_kmeans = input_plio::create("in_plio_kmeans", plio_32_bits, "data/in_plio_source_1.txt");
			out_kmeans = output_plio::create("out_plio_kmeans", plio_32_bits, "data/out_plio_sink_1.txt");

			// ------kernel connection------
			connect<stream>(in_kmeans.out[0], kmeans_kernel.in[0]);
			connect<stream>(kmeans_kernel.out[0], out_kmeans.in[0]);

			// set kernel source and headers
			source(kmeans_kernel) = "src/kmeans_kernel.cpp";
			headers(kmeans_kernel) = {"src/kmeans_kernel.h", "../common/common.h"};

			// set ratio
			runtime<ratio>(kmeans_kernel) = 0.9;
	};
};
