#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "precomp.h"
#include "head.h"

#define SECTOR_SIZE 512
#define MAX_SECTORS 104857600
#define EXT4_SUPERBLOCK_OFFSET 1024
#define EXT4_SIGNATURE 0x53ef
void read_ext4(const char *device)
{
    int fd = open(device, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }

    uint8_t sector[SECTOR_SIZE];
    int ext4_count = 0;

    FILE *output_file = fopen("ext4_info.txt", "w");
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

        uint8_t signature[2] = {sector[0x01F8], sector[0x01F9]};
        uint16_t superblock_signature = (signature[0] << 8) | signature[1];
        if (superblock_signature == EXT4_SIGNATURE) {
            fprintf(output_file, "EXT4 Superblock found at sector %ld\n", sector_num);
            printf("EXT4 Superblock found at sector %ld\n", sector_num);

            uint32_t block_size = *(uint32_t *)&sector[0x18]; // Superblock size
            uint32_t inode_size = *(uint32_t *)&sector[0x38]; // inode size
            uint32_t block_count = *(uint32_t *)&sector[0x04]; // block
            uint32_t inode_count = *(uint32_t *)&sector[0x08]; // inode

            fprintf(output_file, "Metadata:\n");
            printf("Metadata:\n");
            fprintf(output_file, "Block size: %u bytes\n", block_size);
            printf("Block size: %u bytes\n", block_size);
            fprintf(output_file, "Inode size: %u bytes\n", inode_size);
            printf("Inode size: %u bytes\n", inode_size);
            fprintf(output_file, "Block count: %u\n", block_count);
            printf("Block count: %u\n", block_count);
            fprintf(output_file, "Inode count: %u\n", inode_count);
            printf("Inode count: %u\n", inode_count);

            uint32_t block_group_descriptor = *(uint32_t *)&sector[0x20];
            fprintf(output_file, "Block group descriptor: %u\n", block_group_descriptor);
            printf("Block group descriptor: %u\n", block_group_descriptor);

            ext4_count++;
        }
    }

    if (ext4_count == 0) {
        fprintf(output_file, "No EXT4 Superblock found in the first %d sectors\n", MAX_SECTORS);
        printf("No EXT4 Superblock found in the first %d sectors\n", MAX_SECTORS);
    } else {
        fprintf(output_file, "Total EXT4 Superblocks found: %d\n", ext4_count);
        printf("Total EXT4 Superblocks found: %d\n", ext4_count);
    }

    fclose(output_file);
    close(fd);
}
