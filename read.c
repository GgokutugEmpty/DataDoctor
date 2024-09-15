/*
 * Sector Reader Utility
 * ====================================
 * Author: A.Goktug
 * Description: This program reads a specific sector from a given block device and prints
 *              the sector data in hexadecimal format.
 * Version: 1.0
 * Date: 2024-09-15
 * 
 * Notes:
 * - Ensure you have the necessary permissions to access the block device.
 * - The device path should be specified correctly to avoid errors.
 */
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

void print_hex(const uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%02x ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}


void read_sector(const char *device, off_t sector, size_t sector_size) {
    int fd = open(device, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }

    if (lseek(fd, sector * sector_size, SEEK_SET) < 0) {
        perror("lseek");
        close(fd);
        return;
    }

    uint8_t buffer[sector_size];
    ssize_t bytes_read = read(fd, buffer, sector_size);
    if (bytes_read < 0) {
        perror("read");
        close(fd);
        return;
    } else if (bytes_read == 0) {
        printf("No data read from sector\n");
        close(fd);
        return;
    }

    // Print the sector data in hexadecimal format
    printf("Sector %ld data:\n", sector);
    for (size_t i = 0; i < sector_size; i++) {
        printf("%02x ", buffer[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");

    close(fd);
}
