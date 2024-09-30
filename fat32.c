#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "precomp.h"

#define SECTOR_SIZE 512
#define MAX_SECTORS 104857600



void read_fat32(const char *device) {
    int fd = open(device, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }

    uint8_t sector[SECTOR_SIZE];
    int fat32_count = 0;

    for (off_t sector_num = 0; sector_num < MAX_SECTORS; sector_num++) {
        if (lseek(fd, sector_num * SECTOR_SIZE, SEEK_SET) < 0) {
            perror("lseek");
            close(fd);
            return;
        }
        if (read(fd, sector, SECTOR_SIZE) != SECTOR_SIZE) {
            perror("read");
            close(fd);
            return;
        }

        uint8_t signature[2] = {sector[0x1FE], sector[0x1FF]};
        if (signature[0] == 0x55 && signature[1] == 0xAA) {
            uint8_t fs_type[8];
            memcpy(fs_type, &sector[0x52], 8);
            if (strncmp((char *)fs_type, "FAT32   ", 8) == 0) {
                printf("FAT32 Boot Sector found at sector %ld\n", sector_num);
                print_hex(sector, SECTOR_SIZE);
                uint8_t sectors_per_cluster = sector[0x0D]; // 1 byte
                uint16_t reserved_sector_count = *(uint16_t *)&sector[0x0E]; // 2 bytes
                uint8_t number_of_fats = sector[0x10]; // 1 byte
                uint32_t fat_size_32 = *(uint32_t *)&sector[0x24]; // 4 bytes
                uint32_t root_dir_first_cluster = *(uint32_t *)&sector[0x2C]; // 4 bytes

                printf("Metadata:\n");
                printf("Sectors per cluster: %u\n", sectors_per_cluster);
                printf("Reserved sectors: %u\n", reserved_sector_count);
                printf("Number of FATs: %u\n", number_of_fats);
                printf("FAT size (in sectors): %u\n", fat_size_32);
                printf("Root directory first cluster: %u\n", root_dir_first_cluster);
                uint32_t root_dir_start_sector = reserved_sector_count + (number_of_fats * fat_size_32) +
                (root_dir_first_cluster - 2) * sectors_per_cluster;

                off_t root_dir_offset = root_dir_start_sector * SECTOR_SIZE;
                uint8_t root_dir_sector[SECTOR_SIZE];

                if (lseek(fd, root_dir_offset, SEEK_SET) < 0) {
                    perror("lseek (root dir)");
                    close(fd);
                    return;
                }
                if (read(fd, root_dir_sector, SECTOR_SIZE) != SECTOR_SIZE) {
                    perror("read (root dir)");
                    close(fd);
                    return;
                }

                printf("\nRoot Directory Entries (First 512 Bytes):\n");
                print_hex(root_dir_sector, SECTOR_SIZE);

                fat32_count++;
            }
        }
    }

    if (fat32_count == 0) {
        printf("FAT32 Boot Sector not found in the first %d sectors\n", MAX_SECTORS);
    } else {
        printf("Total FAT32 Boot Sectors found: %d\n", fat32_count);
    }

    close(fd);
}
