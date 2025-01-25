#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <ap_axi_sdata.h>
#include "../sink_from_aie.cpp"
#include <cmath>

int main(int argc, char *argv[])
{
    hls::stream<ap_uint<sizeof(float) * 8 * 8>> s;
    int32_t num_clusters = 4;
    int32_t buffer_size = num_clusters * 2;
    std::vector<float> buffer(buffer_size, 0);

    // Read the output of the AIE kernel from a file
    std::ifstream file("../../aie/x86simulator_output/data/out_plio_sink_1.txt", std::ios::in);

    if (!file)
    {
        std::cerr << "Unable to open file ../../aie/x86simulator_output/data/out_plio_sink_1.txt";
        return 1;
    }

    // Read packed data into the input stream
    for (size_t i = 0; i < num_clusters; i++)
    {
        ap_uint<sizeof(float) * 8 * 8> packed_data = 0;

        // Read 8 floats for each packed data entry
        for (size_t j = 0; j < 8; j++) 
        {
            float value;
            file >> value;
            packed_data.range(j * 32 + 31, j * 32) = *reinterpret_cast<uint32_t *>(&value);
        }
        s.write(packed_data);
    }

    std::cout << "Reading done" << std::endl;

    sink_from_aie(s, buffer.data(), num_clusters);
    std::cout << "Sink from AIE done" << std::endl;

    // Print the output of the AIE kernel
    for (size_t i = 0; i < buffer_size; i++)
    {
        std::cout << "Output " << i << ": " << buffer[i] << std::endl;
    }

    return 0;
}