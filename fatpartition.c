/*
 * Partition Scanner Utility
 * ====================================
 * Author: A.Goktug
 * Description: This program scans a block device for Master Boot Record (MBR) partitions.
 *              It reads sectors to identify MBR signatures and prints partition information.
 * Version: 1.0
 * Date: 2024-09-15
 * 
 * Notes:
 * - Ensure the device path is correct and accessible.
 * - The program scans up to MAX_SECTORS to find MBR partitions.
 */


#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "precomp.h"

#define SECTOR_SIZE 512
#define MAX_SECTORS 104857600
#define PARTITION_TABLE_OFFSET 0x1BE

typedef struct {
    uint8_t  boot_indicator;
    uint8_t  start_head;
    uint8_t  start_sector;
    uint8_t  start_cylinder;
    uint8_t  partition_type;
    uint8_t  end_head;
    uint8_t  end_sector;
    uint8_t  end_cylinder;
    uint32_t start_lba;
    uint32_t num_sectors;
} __attribute__((packed)) PartitionEntry;

void print_partition_info(const PartitionEntry *entry, off_t start_sector) {
    printf("Partition type: 0x%02x\n", entry->partition_type);
    printf("Start sector: %u\n", entry->start_lba);
    printf("Number of sectors: %u\n", entry->num_sectors);
    printf("Partition offset: %ld\n", start_sector);
    printf("\n");
}

void partition_scan(const char *device) {
    int fd = open(device, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }

    uint8_t sector[SECTOR_SIZE];
    int partition_count = 0;

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
            printf("MBR Signature found at sector %ld\n", sector_num);

            PartitionEntry *partitions = (PartitionEntry *)&sector[PARTITION_TABLE_OFFSET];
            for (int i = 0; i < 4; i++) {
                PartitionEntry *entry = &partitions[i];
                if (entry->partition_type != 0x00) {
                    printf("Partition %d:\n", i + 1);
                    print_partition_info(entry, sector_num);
                    partition_count++;
                }
            }
        }
    }

    if (partition_count == 0) {
        printf("No partitions found in the first %d sectors\n", MAX_SECTORS);
    } else {
        printf("Total partitions found: %d\n", partition_count);
    }

    close(fd);
}
