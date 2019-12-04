.PHONY: clean

TARGET = chr-rom-viewer
BUILD  = build
CFILES = $(filter-out ., $(shell find . -name "*.c" -maxdepth 1))
OFILES = $(addprefix $(BUILD)/, $(subst ./, , $(CFILES:.c=.o)))

COMPILER = clang
CFLAGS   += -Wall `sdl2-config --cflags`
LDFLAGS  += `sdl2-config --libs`

default:
	@[ -d $(BUILD) ] || mkdir -p $(BUILD)
	@make all

all: $(TARGET)

$(TARGET): $(OFILES)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILD)/%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD)/rom.o: rom.h
$(BUILD)/main.o: rom.h

clean:
	rm -f $(BUILD)/*
	rm -f $(TARGET)
