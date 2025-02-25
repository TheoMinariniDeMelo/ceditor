CC = gcc
CFLAGS = -Wall -g
VPATH = src

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c)   # Pega todos os .c dentro de src/
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS)) # nesse caso o wildcard % salva o resultado e envia para o segundo argumento

TARGET = a.out

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
#$< pega a primeira dependencia, $@ pega o target
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
