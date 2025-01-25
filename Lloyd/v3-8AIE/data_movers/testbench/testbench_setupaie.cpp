#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <ap_axi_sdata.h>
#include <cmath>
#include "../setup_aie.cpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>

void read_file(const char *filename, hls::stream<ap_uint<sizeof(float) * 8 * 8>> &s, int32_t input_size)
{
    std::ofstream file;
    file.open(filename);

    if (file.is_open())
    {
        // Write the number of clusters and points
        ap_uint<sizeof(float) * 8 * 8> tmp = s.read();
        for (size_t i = 0; i < 8; i++)
        {
            uint32_t value = tmp.range(i * 32 + 31, i * 32);
            file << value << std::endl;
            std::cout << value << std::endl;
        }

        // Write the clusters and points coordinates
        for (size_t i = 0; i < (input_size / 8); i++)
        {
            tmp = s.read();

            for (size_t j = 0; j < 8; j++)
            {
                uint32_t float_bits = tmp.range(j * 32 + 31, j * 32);
                float value = *reinterpret_cast<float *>(&float_bits);

                file << value << std::endl;
                std::cout << value << std::endl;
            }
        }

        file.close();
    }
    else
    {
        std::cout << "Error opening file" << std::endl;
    }
}

int main(int argc, char* argv[]) 
{
    hls::stream<ap_uint<sizeof(float) * 8 * 8>> s1;
    hls::stream<ap_uint<sizeof(float) * 8 * 8>> s2;
    std::srand(time(nullptr));

    // Define number of clusters and points
    int32_t num_clusters = 4;
    int32_t num_points = 8;
    int32_t input_size = (num_clusters + num_points) * 2;
    int32_t input_size_file = num_clusters * 2 + num_points;

    // Prepare the input buffer
    std::vector<float> input_buffer = {3.0, -7.0, -8.5, 5.0, 10.0, -3.0, -4.0, -6.0, 7.0, 9.0, -2.0, 0.0, 5.0, 4.0, -10.0, -8.0, 6.0, -2.0, -3.0, 7.0, 1.0, -9.0, 9.0, 3.0};
    std::vector<ap_uint<sizeof(float) * 8 * 8>> input_buffer_packed(input_size / 8);

    // Pack the input floats into ap_uint<256>
    for (size_t i = 0; i < input_buffer.size(); i += 8)
    {
        ap_uint<sizeof(float) * 8 * 8> packed_data = 0;

        for (size_t j = 0; j < 8; j++)
        {
            if (i + j < input_buffer.size())
            {
                uint32_t float_bits = *reinterpret_cast<uint32_t *>(&input_buffer[i + j]);
                packed_data.range(j * 32 + 31, j * 32) = float_bits;
            }
        }

        input_buffer_packed[i / 8] = packed_data;
    }

    setup_aie(num_clusters, num_points, input_buffer_packed.data(), s1, s2);

    // Read the stream and write to file
    read_file("../../aie/data/in_plio_source_1.txt", s1, input_size_file);
    read_file("../../aie/data/in_plio_source_2.txt", s2, input_size_file);

    return 0;
}
