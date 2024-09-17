#include <iostream>
#include <fstream>
#include <iomanip>
#include <sys/stat.h>
#include <cstring> // For strcpy, strcat
extern "C"
{
// Dosya boyutunu ve diğer bilgileri almak için kullanılan fonksiyon
void print_file_metadata(const std::string& filepath) {
    struct stat file_stat;
    if (stat(filepath.c_str(), &file_stat) == 0) {
        std::cout << "File: " << filepath << std::endl;
        std::cout << "Size: " << file_stat.st_size << " bytes" << std::endl;
        std::cout << "Last modified: " << ctime(&file_stat.st_mtime); // Tarih formatında
    } else {
        std::cerr << "Error getting file metadata: " << filepath << std::endl;
    }
}

// Hexadecimal formatında verileri yazdıran fonksiyon
void print_hexF(const char* data, std::size_t size) {
    for (std::size_t i = 0; i < size; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') 
                  << (static_cast<int>(static_cast<unsigned char>(data[i]))) << ' ';
        if ((i + 1) % 16 == 0) std::cout << '\n';
    }
    std::cout << std::dec << std::endl; // Hex modundan sonra tekrar decimal moda dön
}

// Dosyanın hex formatını çıkaran ana fonksiyon
void hex_dump(const std::string& filepath) {
std::ifstream file(filepath, std::ios::binary);
if (!file) {
    std::cerr << "Error opening file: " << strerror(errno) << std::endl;
    return;
}

    print_file_metadata(filepath); // Dosya metadatalarını yazdır
    
    const std::size_t buffer_size = 512; // 512 baytlık bloklar halinde okuyalım
    char buffer[buffer_size];

    std::size_t offset = 0;
    while (file.read(buffer, buffer_size) || file.gcount() > 0) {
        std::size_t read_bytes = file.gcount(); // Kaç bayt okunduğunu öğren
        std::cout << "Offset " << std::setw(8) << std::setfill('0') << offset << " | ";
        print_hexF(buffer, read_bytes); // Veriyi hex formatında yazdır
        offset += read_bytes;
    }

    file.close();
}
}