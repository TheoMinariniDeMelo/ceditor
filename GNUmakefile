#CC = gcc
#CFLAGS = -Wall -g
#VPATH = src

#SRC_DIR = src 
#OBJ_DIR = obj

#SRCS = $(wildcard $(SRC_DIR)/*.c)   # Pega todos os .c dentro de src/
#OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS)) # nesse caso o wildcard % salva o resultado e envia para o segundo argumento
#OBJS_CONTROLS = $(patsubst $(SRC_DIR)/controls/%.c, $(OBJ_DIR)/)
#TARGET = a.out

#all: $(TARGET)

#$(TARGET): $(OBJS)
#	$(CC) $(CFLAGS) $(OBJS) -lm -o $(TARGET)

#$(OBJ_DIR)/%.o: %.c
#	mkdir -p $(OBJ_DIR)
#	$(CC) $(CFLAGS) -c $< -o $@
#$< pega a primeira dependencia, $@ pega o target
#clean:
#	rm -rf $(OBJ_DIR) $(TARGET)
#debugger:
#	$(CC) $(CFLAGS) -g $(OBJS) -o d-$(TARGET)
#.PHONY: all clean

SRC = ./src


all: compile

compile: $(SRC)/main.c $(SRC)/terminal_config.c $(SRC)/utils/split_array.c	
	gcc $(SRC)/main.c $(SRC)/terminal_config.c $(SRC)/utils/split_array.c -o a.out
