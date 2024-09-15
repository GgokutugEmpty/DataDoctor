#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

extern "C" {

constexpr size_t BUFFER_SIZE = 4096;

void print_hexf(const char* data, size_t size) 
{
    for (size_t i = 0; i < size; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (static_cast<int>(static_cast<unsigned char>(data[i]))) << ' ';
        if ((i + 1) % 16 == 0) std::cout << '\n';
    }
    std::cout << std::dec << std::endl;
}

void hex_dump(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return;
    }

    std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::cout << "File size: " << file_size << " bytes" << std::endl;
    std::cout << "Hex Dump:\n";

    char buffer[BUFFER_SIZE];
    while (file) {
        file.read(buffer, BUFFER_SIZE);
        std::streamsize bytes_read = file.gcount();

        if (bytes_read > 0) {
            print_hexf(buffer, static_cast<size_t>(bytes_read));
        }
    }

    if (file.eof()) {
        std::cout << "End of file reached.\n";
    } else {
        std::cerr << "Error reading file.\n";
    }
}

void hex_extract(const char* file_path) {
    hex_dump(file_path);
}

}
