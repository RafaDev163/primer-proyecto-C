# Nombre del ejecutable
TARGET = to_do_list

# Directorios
SRC_DIR = src
INC_DIR = include

# Archivos fuente y objeto
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:.c=.o)

# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -I$(INC_DIR)

# Regla por defecto
all: $(TARGET)

# Compilación del ejecutable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

# Compilación de cada .c
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpieza
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)

.PHONY: all clean
