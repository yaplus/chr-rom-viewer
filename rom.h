#ifndef ROM_H
#define ROM_H

#include <stdint.h>

#define ROM_LOAD_FAILED     (-1)

typedef enum {
    NTSC, PAL
} TV_SYSTEM;

typedef enum {
    HORIZONTAL, VERTICAL
} MIRROR_TYPE;

uint8_t* ROM_PRG;   // PRG ROM
uint8_t* ROM_CHR;   // CHR ROM

int rom_init(const char* filename);

unsigned int rom_get_size_prg();
unsigned int rom_get_size_chr();
MIRROR_TYPE rom_get_mirroring();
int rom_get_has_sram();
int rom_get_has_trainer();
int rom_get_is_four_screen();

#endif /* ROM_H */
