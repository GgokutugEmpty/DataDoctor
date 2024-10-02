#include <stdio.h>
#include <stdlib.h>

void list_block_devices() {
    FILE *fp;
    char buffer[256];

    // /proc/partitions dosyasını aç
    fp = fopen("/proc/partitions", "r");
    if (fp == NULL) {
        perror("Failed to open /proc/partitions");
        return;
    }

    // Başlık satırını geç
    fgets(buffer, sizeof(buffer), fp);
    fgets(buffer, sizeof(buffer), fp);

    // Cihazları oku ve yazdır
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Cihaz bilgilerini ayıkla
        unsigned long size;
        char device[32];

        sscanf(buffer, "%lu %*d %*d %s", &size, device);
        printf("Device: %s, Size: %lu KB\n", device, size);
    }

    fclose(fp);
}


