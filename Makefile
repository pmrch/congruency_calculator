CC = clang

STRICT_FLAGS = -Weverything -Werror -Wno-padded -Wno-unsafe-buffer-usage -Wno-declaration-after-statement
OPT_FLAGS = -O3 -march=native -DNDEBUG -flto
LDFLAGS = -flto -ffast-math -static

ALL_SRC := $(shell find src -name '*.c')
SRC := $(filter-out src/%_win32.c, $(ALL_SRC))
OBJ := $(patsubst src/%.c,build/%.o,$(SRC))

CFLAGS = $(OPT_FLAGS) -Iinclude -MMD -MP $(STRICT_FLAGS)
TARGET = congruency_calculator

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

clean:
	@rm -rf build/*
	@rm -rf $(TARGET)

cdb:
	@make clean
	@bear -- make all

all: $(TARGET)
.PHONY: all clean
