# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Lista de fontes
SRCS = manipulacao_estruturas.c
HEADERS = estrutura_ext.h manipulacao_estruturas.h
OBJS = $(SRCS:.c=.o)

# Nome do executável
TARGET = programa

# Regra padrão para compilar o programa
all: $(TARGET)

# Regra para compilar o programa
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Regra para compilar os objetos
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ -lm

# Limpar arquivos intermediários e o executável
clean:
	rm -f $(OBJS) $(TARGET)
