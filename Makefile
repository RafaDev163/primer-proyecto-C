# =========================
# Configuración base
# =========================
CC       = gcc
SRC_DIR  = src
INC_DIR  = include

# Detectar Windows para .exe
EXE =
ifeq ($(OS),Windows_NT)
  EXE = .exe
endif

TARGET   = to_do_list$(EXE)
SRC      = $(wildcard $(SRC_DIR)/*.c)
OBJ      = $(SRC:.c=.o)

# Flags comunes
CSTD     = -std=c11
WARN     = -Wall -Wextra -Wpedantic
INCLUDES = -I$(INC_DIR)

# Compilación normal (debug ligera)
CFLAGS   = $(CSTD) $(WARN) $(INCLUDES) -O0 -g
LDFLAGS  =

# AddressSanitizer (se añade en el target 'asan')
ASAN_CFLAGS  = -O0 -g -fsanitize=address -fno-omit-frame-pointer
ASAN_LDFLAGS = -fsanitize=address

# =========================
# Reglas
# =========================
.PHONY: all clean run asan run-asan print

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Regla de compilación de cada .c -> .o
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Ejecutar binario normal
run: all
	./$(TARGET)

# Build con ASan
asan: CFLAGS  += $(ASAN_CFLAGS)
asan: LDFLAGS += $(ASAN_LDFLAGS)
asan: clean $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Ejecutar con ASan
run-asan: asan
	./$(TARGET)

# Limpiar
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)

# Depuración del Makefile (opcional)
print:
	@echo OS=$(OS)
	@echo EXE=$(EXE)
	@echo TARGET=$(TARGET)
	@echo SRC=$(SRC)
	@echo OBJ=$(OBJ)
	@echo CFLAGS=$(CFLAGS)
	@echo LDFLAGS=$(LDFLAGS) 
