# DataDoctor Utility Program

## Description
The DataDoctor Utility Program provides several utilities for interacting with block devices and filesystems on a Linux-based system. The utilities included are:

- [X] **Block Device Listing Utility** [X] : Lists all block devices currently available on the system.
- [X] **FAT32 Reader Utility** [X] : Scans a block device for FAT32 boot sectors and displays metadata and root directory entries.
- [X] **Partition Scanner Utility** [X] : Scans a block device for Master Boot Record (MBR) partitions and prints partition information.
- [X] **Sector Reader Utility** : Reads a specific sector from a block device and prints the sector data in hexadecimal format.

## Author
A.Goktug

## Version
1.0

## Date
2024-09-15

## Features

### Block Device Listing Utility
- [X] Lists all block devices by reading the `/sys/class/block` directory and identifying symbolic links to block devices.

### FAT32 Reader Utility
- [X] **
- Scans a block device for FAT32 boot sectors.
- Extracts and prints metadata such as sectors per cluster, reserved sectors, number of FATs, FAT size, and root directory first cluster.
- Displays the root directory entries from the FAT32 filesystem.

### Partition Scanner Utility
- [X] **
- Scans a block device for MBR partitions.
- Identifies partitions and prints their type, start sector, number of sectors, and offset.

### Sector Reader Utility
- [X] **
- Reads a specific sector from a block device and prints the data in hexadecimal format.

## Planned Features

### NTFS Reader Utility
- [ ] (Upcoming)
- Planned to scan a block device for NTFS boot sectors.
- Will extract and display NTFS metadata such as volume information and file system properties.

### EXT4 Reader Utility
- [ ] (Upcoming)
- Planned to scan a block device for EXT4 boot sectors.
- Will extract and display EXT4 metadata, including superblock information and file system structure.

### Data Recovery Utility
- [ ] (Upcoming)
- Planned to provide functionality for recovering lost or deleted files from FAT32, NTFS, and EXT4 filesystems.
- Will include options for scanning and restoring files from various file systems.

## Requirements
- The program requires appropriate permissions to access block devices.
- Ensure that the device path specified in the program is correct and accessible.

## Disclaimers and Limitations
- **Data Loss**: The program performs low-level operations on block devices. Misuse or incorrect input may result in data loss or corruption. Use with caution and ensure you have backups of important data.
- **System Compatibility**: Designed for Linux-based systems. May not work correctly on other operating systems or non-standard hardware.
- **Permissions**: Some operations may require elevated privileges (e.g., root access). Ensure you have the necessary permissions.
- **Errors and Bugs**: The program is provided as-is. There may be bugs or errors. The author is not responsible for any issues arising from the use of this program.
- **No Warranty**: Distributed without any warranties of any kind.

## Build Instructions

To build the program, use `make`:

```bash
make
