# Nome do executável
TARGET = street_fighter

# Diretório das fontes
SRC_DIR = src

# Arquivos fontes
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/fighter.c $(SRC_DIR)/joystick.c $(SRC_DIR)/display.c $(SRC_DIR)/hadouken.c  $(SRC_DIR)/power.c  $(SRC_DIR)/fighter_visuals.c 

# Arquivos objeto (gerados a partir dos fontes)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(SRC_DIR)/%.o, $(SRCS))

# Flags padrão de compilação
CFLAGS = -Wall -Wextra -std=c11

# Flags e bibliotecas do Allegro
ALLEGRO_FLAGS = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 --libs --cflags) -lm

# Compilador
CC = gcc

# Regra padrão: compilar tudo
all: $(TARGET)

# Regra para compilar o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(ALLEGRO_FLAGS)

# Regra para compilar os arquivos fonte em arquivos objeto
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< $(ALLEGRO_FLAGS)

# Limpar os arquivos objeto
clean:
	rm -f $(OBJS)

# Limpar tudo, incluindo o executável
purge: clean
	rm -f $(TARGET)

# Regra para forçar a recompilação
.PHONY: all clean purge


