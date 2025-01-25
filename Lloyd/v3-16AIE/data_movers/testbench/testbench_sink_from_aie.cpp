#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <ap_axi_sdata.h>
#include "../sink_from_aie.cpp"
#include <cmath>

void read_file(const char *filename, hls::stream<float> &s, int32_t buffer_size) {
    std::ifstream file;
    file.open(filename);

    if (!file)
    {
        std::cerr << "Unable to open file " << filename;
        return;
    }

    for (size_t i = 0; i < buffer_size; i++)
    {
        for (size_t j = 0; j < 8; j++) 
        {
            float x;
            file >> x;
            std::cout << "Read " << x << std::endl;
            s.write(x);
        }
    }
}

int main(int argc, char *argv[])
{
    hls::stream<float> s1;
    hls::stream<float> s2;
    int32_t num_clusters = 4;
    int32_t buffer_size = num_clusters * 2;
    std::vector<float> buffer(buffer_size, 0);

    // Read the output of the AIE kernels from their respective files
    read_file("../../aie/x86simulator_output/data/out_plio_sink_1.txt", s1, num_clusters);
    read_file("../../aie/x86simulator_output/data/out_plio_sink_2.txt", s2, num_clusters);

    std::cout << "Reading done" << std::endl;
    sink_from_aie(s1, s2, buffer.data(), num_clusters);
    std::cout << "Sink from AIE done" << std::endl;

    // Print the output of the AIE kernel
    for (size_t i = 0; i < buffer_size; i++)
    {
        std::cout << "Output " << i << ": " << buffer[i] << std::endl;
    }
}