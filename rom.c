#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h> /* for strncmp */
#include "rom.h"

static uint8_t  mapper;         // Mapper#
static unsigned int pages_prg;
static unsigned int pages_chr;
static unsigned int size_prg;
static unsigned int size_chr;
static uint8_t  trainer[512];   // Trainer (if needed)

static MIRROR_TYPE mirroring; // Mirroring type
static int is_four_screen;  // Is 4-screen
static int has_sram;        // has SRAM or not (battery-backed)
static int has_trainer;     // has Trainer or not

static TV_SYSTEM video;       // Video standard type (NTSC/PAL)

unsigned int rom_get_size_prg() {
    return size_prg;
}

unsigned int rom_get_size_chr() {
    return size_chr;
}

MIRROR_TYPE rom_get_mirroring() {
    return mirroring;
}

int rom_get_has_sram() {
    return has_sram;
}

int rom_get_has_trainer() {
    return has_trainer;
}

int rom_get_is_four_screen() {
    return is_four_screen;
}

int rom_init(const char* filename)
{
    FILE *fp = fopen(filename,"rb");
    if (fp == NULL) {
        fprintf(stderr, "Can't open file : %s\n", filename);
        return ROM_LOAD_FAILED;
    }

    // Check "NES" identifier
    char identifier[4];
    fread(identifier,1,4,fp);
    if (strncmp(identifier, "NES\x1A", 4) != 0) {
        fclose(fp);
        fprintf(stderr, "Invalid file type\n");
        return ROM_LOAD_FAILED;
    }

    // Load number of ROM pages
    fread(&pages_prg, 1, 1, fp);
    fread(&pages_chr, 1, 1, fp);

    // Load flags
    char fl6,fl7;
    fread(&fl6, 1, 1, fp);
    fread(&fl7, 1, 1, fp);
    mirroring      = (fl6&1) ? VERTICAL : HORIZONTAL;
    has_sram       = (fl6&2) != 0;
    has_trainer    = (fl6&4) != 0;
    is_four_screen = (fl6&8) != 0;
    mapper         = (fl6>>4) | (fl7&0xf0);

    // Skip a byte
    fseek(fp, 1, SEEK_CUR);

    // Load a type of TV System
    char fl9;
    fread(&fl9, 1, 1, fp);
    video = !(fl9&1) ? NTSC : PAL;

    // Skip 6 bytes
    fseek(fp, 6, SEEK_CUR);

    // Load Trainer
    if (has_trainer) {
        fread(trainer, 1, 512, fp);
    }

    // Load ROM body
    size_prg = 0x4000 * pages_prg;
    size_chr = 0x2000 * pages_chr;

    ROM_PRG = malloc(size_prg);
    if (ROM_PRG == NULL) {
        fprintf(stderr, "malloc");
        return ROM_LOAD_FAILED;
    }

    ROM_CHR = malloc(size_chr);
    if (ROM_CHR == NULL) {
        fprintf(stderr, "malloc");
        return ROM_LOAD_FAILED;
    }

    fread(ROM_PRG, 1, size_prg, fp);
    fread(ROM_CHR, 1, size_chr, fp);

    fclose(fp);

    fprintf(stdout, "Mapper No. : %d\n",    (int)mapper);
    fprintf(stdout, "Trainer    : %s\n",    has_trainer ? "Yes" : "No");
    fprintf(stdout, "PRG-ROM    : %d kB\n", (int)size_prg/1024);
    fprintf(stdout, "CHR-ROM    : %d kB\n", (int)size_chr/1024);
    fprintf(stdout, "Mirroring  : %s\n",    mirroring == VERTICAL ? "vertical" : "horizontal");
    fprintf(stdout, "TV system  : %s\n",    video == NTSC ? "NTSC" : "PAL");
    fprintf(stdout, "4-screen   : %s\n",    is_four_screen ? "Yes" : "No");
    fprintf(stdout, "SRAM       : %s\n",    has_sram ? "Yes" : "No");

    return 0;
}
