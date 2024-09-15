#ifndef PRECOMP_H
#define PRECOMP_H
void read_sector(const char *device, off_t sector, size_t sector_size);
void read_fat32(const char *device);
void partition_scan(const char *device);
void print_hex(const uint8_t *data, size_t size);
void list_block_devices();
#endif