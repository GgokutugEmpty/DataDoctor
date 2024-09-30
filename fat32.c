#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "precomp.h"
#include "head.h"
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

    FILE *output_file = fopen("fat32_info.txt", "w");
    if (!output_file) {
        perror("fopen");
        close(fd);
        return;
    }

    for (off_t sector_num = 0; sector_num < MAX_SECTORS; sector_num++) {
        if (lseek(fd, sector_num * SECTOR_SIZE, SEEK_SET) < 0) {
            perror("lseek");
            close(fd);
            fclose(output_file);
            return;
        }
        if (read(fd, sector, SECTOR_SIZE) != SECTOR_SIZE) {
            perror("read");
            close(fd);
            fclose(output_file);
            return;
        }

        uint8_t signature[2] = {sector[0x1FE], sector[0x1FF]};
        if (signature[0] == 0x55 && signature[1] == 0xAA) {
            uint8_t fs_type[8];
            memcpy(fs_type, &sector[0x52], 8);
            if (strncmp((char *)fs_type, "FAT32   ", 8) == 0) {
                fprintf(output_file, "FAT32 Boot Sector found at sector %ld\n", sector_num);
                printf("FAT32 Boot Sector found at sector %ld\n", sector_num);
                uint8_t sectors_per_cluster = sector[0x0D];
                uint16_t reserved_sector_count = *(uint16_t *)&sector[0x0E];
                uint8_t number_of_fats = sector[0x10];
                uint32_t fat_size_32 = *(uint32_t *)&sector[0x24];
                uint32_t root_dir_first_cluster = *(uint32_t *)&sector[0x2C];

                fprintf(output_file, "Metadata:\n");
                printf("Metadata:\n");
                fprintf(output_file, "Sectors per cluster: %u\n", sectors_per_cluster);
                printf("Sectors per cluster: %u\n", sectors_per_cluster);
                fprintf(output_file, "Reserved sectors: %u\n", reserved_sector_count);
                printf("Reserved sectors: %u\n", reserved_sector_count);
                fprintf(output_file, "Number of FATs: %u\n", number_of_fats);
                printf("Number of FATs: %u\n", number_of_fats);
                fprintf(output_file, "FAT size (in sectors): %u\n", fat_size_32);
                printf("FAT size (in sectors): %u\n", fat_size_32);
                fprintf(output_file, "Root directory first cluster: %u\n", root_dir_first_cluster);
                printf("Root directory first cluster: %u\n", root_dir_first_cluster);

                uint32_t root_dir_start_sector = reserved_sector_count + (number_of_fats * fat_size_32) +
                (root_dir_first_cluster - 2) * sectors_per_cluster;

                off_t root_dir_offset = root_dir_start_sector * SECTOR_SIZE;
                uint8_t root_dir_sector[SECTOR_SIZE];

                if (lseek(fd, root_dir_offset, SEEK_SET) < 0) {
                    perror("lseek (root dir)");
                    close(fd);
                    fclose(output_file);
                    return;
                }
                if (read(fd, root_dir_sector, SECTOR_SIZE) != SECTOR_SIZE) {
                    perror("read (root dir)");
                    close(fd);
                    fclose(output_file);
                    return;
                }

                fprintf(output_file, "\nRoot Directory Entries (First 512 Bytes):\n");
                printf("\nRoot Directory Entries (First 512 Bytes):\n");
                print_hex_ascii(root_dir_sector, SECTOR_SIZE, output_file);

                char volume_label[12] = {0};
                for (int i = 0; i < 11; i++) {
                    volume_label[i] = root_dir_sector[i + 0x2B];
                }

                fprintf(output_file, "Found Volume Label: %.11s\n", volume_label);
                printf("Found Volume Label: %.11s\n", volume_label);

                fat32_count++;
            }
        }
    }

    if (fat32_count == 0) {
        fprintf(output_file, "FAT32 Boot Sector not found in the first %d sectors\n", MAX_SECTORS);
        printf("FAT32 Boot Sector not found in the first %d sectors\n", MAX_SECTORS);
    } else {
        fprintf(output_file, "Total FAT32 Boot Sectors found: %d\n", fat32_count);
        printf("Total FAT32 Boot Sectors found: %d\n", fat32_count);
    }

    fclose(output_file);
    close(fd);
}
