/*
 * Block Device Listing Utility
 * ====================================
 * Author: A.Goktug
 * Description: This program lists all block devices currently available on the system.
 *              It reads the `/sys/class/block` directory, identifies symbolic links to
 *              block devices, and prints their real paths.
 * Version: 1.0
 * Date: 2024-09-15
 * 
 * Notes:
 * - Requires appropriate permissions to access `/sys/class/block`.
 * - The output will include paths of block devices.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>  

#define BLOCK_DEV_DIR "/sys/class/block"
// FIX!
void list_block_devices() {
    struct dirent *entry;
    DIR *dp = opendir(BLOCK_DEV_DIR);

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    printf("List of block devices:\n");

    while ((entry = readdir(dp))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[256];
        snprintf(path, sizeof(path), "%s/%s", BLOCK_DEV_DIR, entry->d_name);

        struct stat st;
        if (stat(path, &st) == 0 && S_ISLNK(st.st_mode)) {
            char real_path[256];
            ssize_t len = readlink(path, real_path, sizeof(real_path) - 1);
            if (len != -1) {
                real_path[len] = '\0';
                printf("%s\n", real_path);
            }
        }
    }

    closedir(dp);
}
