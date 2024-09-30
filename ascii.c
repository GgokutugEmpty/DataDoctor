#include <stdio.h>
#include <stdint.h>

void print_hex_ascii(const uint8_t* buffer, size_t size, FILE *file)
{
    for (size_t i = 0; i < size; i += 16) {
        fprintf(file, "%08lx  ", (unsigned long)i);
        printf("%08lx  ", (unsigned long)i);
        for (size_t j = 0; j < 16; j++) {
            if (i + j < size) {
                fprintf(file, "%02x ", buffer[i + j]);
                printf("%02x ", buffer[i + j]);
            } else {
                fprintf(file, "   ");
                printf("   ");
            }
        }
        fprintf(file, " |");
        printf(" |");
        for (size_t j = 0; j < 16; j++) {
            if (i + j < size) {
                uint8_t ch = buffer[i + j];
                if (ch >= 32 && ch <= 126) {
                    fprintf(file, "%c", ch);
                    printf("%c", ch);
                } else {
                    fprintf(file, ".");
                    printf(".");
                }
            }
        }
        fprintf(file, "|\n");
        printf("|\n");
    }
}
