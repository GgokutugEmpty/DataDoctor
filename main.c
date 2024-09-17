/**++
 * DataDoctor Utility Program
 * ====================================
 * Author: A.Goktug
 * Description: This program allows you to read sectors, scan, find FAT32 filesystems,
 *              scan partitions, and list block devices on a Linux system.
 * Version: 1.0
 * Date: 2024-09-15
 * 
 * Notes:
 * - Make sure to run this program with appropriate permissions to access devices.
 * - The device path "/dev/nvme0n1" is used as a default; adjust it according to your system.
 * 
 * Disclaimers and Limitations:
 * ----------------------------
 * - **Data Loss**: The program performs low-level operations on block devices. Misuse or incorrect input may result in data loss or corruption. Use the program with caution and ensure you have backups of important data.
 * - **System Compatibility**: The program is designed to run on Linux-based systems. It may not work correctly on other operating systems or with non-standard hardware.
 * - **Permissions**: Some operations may require elevated privileges (e.g., root access). Ensure you have the necessary permissions to access and manipulate block devices.
 * - **Errors and Bugs**: The program is provided as-is, and while every effort has been made to ensure its correctness, there may be bugs or errors. The author is not responsible for any issues that arise from the use of this program.
 * - **No Warranty**: The program is distributed without any warranties of any kind, either express or implied, including but not limited to the implied warranties of merchantability and fitness for a particular purpose.
 --**/

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include "precomp.h"
#include "cpp.h"

void display_menu() {
    printf("Menu:\n");
    printf("1. Read Sector\n");
    printf("2. Read FAT32\n");
    printf("3. Scan Partitions\n");
    printf("4. List Block Devices\n");
    printf("5. Exit\n");
	printf("6. File HEX Dump\n");
    printf("Please select an option (1-6): ");
}

void read_sector_input(const char *device) {
    off_t sector;
    size_t sector_size = 512;

    printf("Enter the sector number to read: ");
    if (scanf("%ld", &sector) != 1) {
        printf("Invalid sector number. Please enter a valid integer.\n");
        // Clear invalid input
        while (getchar() != '\n');
        return;
    }

    read_sector(device, sector, sector_size);
}

void display_disclaimer() {
    printf("** WARNING **\n");
    printf("This program performs low-level operations on block devices.\n");
    printf("Misuse or incorrect input may result in data loss or corruption.\n");
    printf("Please ensure you have backups of important data before proceeding.\n");
    printf("\nDisclaimers and Limitations:\n");
    printf("- **Data Loss**: The program performs low-level operations on block devices. Misuse or incorrect input may result in data loss or corruption. Use the program with caution and ensure you have backups of important data.\n");
    printf("- **System Compatibility**: The program is designed to run on Linux-based systems. It may not work correctly on other operating systems or with non-standard hardware.\n");
    printf("- **Permissions**: Some operations may require elevated privileges (e.g., root access). Ensure you have the necessary permissions to access and manipulate block devices.\n");
    printf("- **Errors and Bugs**: The program is provided as-is, and while every effort has been made to ensure its correctness, there may be bugs or errors. The author is not responsible for any issues that arise from the use of this program.\n");
    printf("- **No Warranty**: The program is distributed without any warranties of any kind, either express or implied, including but not limited to the implied warranties of merchantability and fitness for a particular purpose.\n");
    printf("\nDo you accept these terms and wish to continue? (yes/no): ");
}

void hex_dump_input() 
{
    char file_path[256];

    printf("Enter the file path for HEX dump: ");
    scanf("%s", file_path);

    hex_dump(file_path);
}
int main() {
    printf("====================================\n");
    printf("      DataDoctor Utility Program\n");
    printf("====================================\n");
    printf("Author: A.Goktug\n");
    printf("Description: This program allows you to read sectors, scan, find FAT32 filesystems,\n");
    printf("             scan partitions, and list block devices on a Linux system.\n");
    printf("Version: 1.0\n");
    printf("Date: 2024-09-15\n");
    printf("====================================\n\n");
    
    display_disclaimer();



    char response[4];
    if (scanf("%3s", response) != 1 || (strcmp(response, "yes") != 0 && strcmp(response, "no") != 0)) {
        printf("Invalid input. Exiting...\n");
        return 1;
    }

    if (strcmp(response, "no") == 0) {
        printf("You have chosen not to proceed. Exiting...\n");
        return 0;
    }

    const char *device = "/dev/nvme0n1";
    int choice;
    printf("====================================\n");
    printf("      DataDoctor Utility Program\n");
    printf("====================================\n");
    printf("Author: A.Goktug\n");
    printf("Description: This program allows you to read sectors, scan, find FAT32 filesystems,\n");
    printf("             scan partitions, and list block devices on a Linux system.\n");
    printf("Version: 1.0\n");
    printf("Date: 2024-09-15\n");
    printf("====================================\n\n");
    while (1) {
        display_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1 and 5.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                read_sector_input(device);
                break;
            case 2:
                read_fat32(device);
                break;
            case 3:
                partition_scan(device);
                break;
            case 4:
                list_block_devices();
                break;
            case 5:
                printf("Exiting...\n");
                return 0;
				
			case 6:
                printf("HEX Dump...\n");
                hex_dump_input();
				break;
            default:
                printf("Invalid choice. Please select a number between 1 and 5.\n");
                break;
        }
    }

    return 0;
}
