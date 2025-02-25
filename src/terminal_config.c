#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "terminal.h"
struct termios original_terminal;

void disable_raw_mode() {
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal) == -1) {
		perror("tcsetattr");
	}
	clear();
	fflush(stdout);
}
void clear_sig(int i){
	clear();
	exit(1);
}
void enable_raw_mode() {
	struct termios terminal;

	// Captura as configurações do terminal e verifica erro
	if (tcgetattr(STDIN_FILENO, &original_terminal) == -1) {
		perror("tcgetattr oritginal terminal");
		exit(1);
	}

	// Cópia das configurações para modificar
	terminal = original_terminal;
	// Desativa o modo canônico e a exibição de entrada
	terminal.c_lflag &= ~(ICANON | ECHO);

	// Aplica as novas configurações e verifica erro
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminal) == -1) {
		perror("tcsetattr terminal");
		exit(1);
	}
}
void atexit_config(){
	// Registra a função de restauração ao sair do programa
	if (atexit(disable_raw_mode) != 0) {
		perror("atexit disable raw mode function");
		exit(1);
	}
}
