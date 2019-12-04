# NES CHR-ROM Viewer

## Usage

```
$ make
cc -c main.c -o build/main.o -Wall `sdl2-config --cflags`
cc -c rom.c -o build/rom.o -Wall `sdl2-config --cflags`
cc build/main.o build/rom.o -o chr-rom-viewer `sdl2-config --libs`
$ ./chr-rom-viewer romfile.nes
```

## requirement

- SDL2
