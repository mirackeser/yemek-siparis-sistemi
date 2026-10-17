# ─────────────────────────────────────────────────────────────────────────────
#  Yemek Sipariş ve Dağıtım Sistemi — Cross-platform Makefile
# ─────────────────────────────────────────────────────────────────────────────

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -pedantic -g
TARGET  = yemek_sistemi
SRC     = main.c

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o
