#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include "./terminal.h"
#include "./terminal_config.h"

void control(char ch) {
	if (ch == 'q') {
		exit(0);
	}
}

void print_mode(bool is_edit_mode) {
	printf("\033[s");                  // Salva a posição do cursor
	printf("\033[%d;1H", get_max_line()); // Move para a última linha
	printf("\033[K");                  // Limpa a linha antes de escrever
	printf(is_edit_mode ? "Edit Mode" : "Control Mode");
	printf("\033[u");            // Restaura a posição original do cursor
	fflush(stdout);
}

int main() {
	resize_handler(0);
	signal(SIGWINCH, resize_handler);
	enable_raw_mode();
	system("clear");

	char c;
	size_t size = 1;

	char *str = malloc(size + 1); // +1 para '\0'
	if (!str) {
		perror("malloc");
		exit(1);
	}
	str[0] = '\0'; // Inicializa string vazia

	bool control_mode = false;

	while (read(STDIN_FILENO, &c, 1) == 1) {
		if(c != 0x08){
			char* str_tmp = realloc(str, size + 2); // +2 para novo char e '\0'
			if (!str_tmp) {
				perror("realloc");
				free(str);
				exit(1);
			}
			str = str_tmp;
		}
		switch(c) {
			case 0x1B: // Tecla ESC alterna o modo
				control_mode = !control_mode;
				print_mode(control_mode);
				continue;

			case 0x16: // Ctrl+V
				str[size - 1] = '\n';
				break;
			default:
				if (control_mode) {
					control(c);
					print_mode(control_mode);
					continue;
				}
				str[size - 1] = c;
				break;
		}

		str[size] = '\0';
		size++;

		system("clear");
		printf("%s", str);
		print_mode(control_mode); // Exibe o modo depois de limpar a tela
		fflush(stdout);
	}

	free(str);
	return 0;

}

