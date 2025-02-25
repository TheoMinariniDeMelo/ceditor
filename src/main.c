#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include "./terminal_config.h"

void control(char ch) {
	if (ch == 'q') {
		exit(0);
	}
}

void print_mode(bool is_edit_mode) {
	printf("\033[s");                  // Salva a posição do cursor
	gotoxy(1,get_max_line()); // Move para a última linha
	printf("\033[K");                  // Limpa a linha antes de escrever
	printf(is_edit_mode ? "Edit Mode" : "Control Mode");
	printf("\033[u");            // Restaura a posição original do cursor
	fflush(stdout);
}

int main() {
	resize_handler(0);
	signal(SIGWINCH, resize_handler);
	signal(SIGINT, clear_sig);
	enable_raw_mode();
	atexit_config();
	system("clear");

	char c;
	size_t size = 1;

	char *str = malloc(size); // +1 para '\0'
	if (!str) {
		perror("malloc");
		exit(1);
	}
	str[0] = '\0'; // Inicializa string vazia

	bool control_mode = false;

	while (read(STDIN_FILENO, &c, 1) == 1) {
		switch(c) {
			case 0x1B: // Tecla ESC alterna o modo
				control_mode = !control_mode;
				print_mode(control_mode);
				break;
			case 0x7F: // backspace
				if (size <= 1) break; // Não faz nada se a string estiver vazia
				size--; // Reduz o tamanho
				char *str_tmp_b = realloc(str, size); // Redimensiona para o novo tamanho
				if (!str_tmp_b) {
					perror("realloc");
					free(str);
					exit(1);
				}
				str = str_tmp_b;
				str[size - 1] = '\0'; // Coloca o terminador no novo final
				break;
			case 0x16: // Ctrl+V
				str[size] = '\n';
				break;
			default:
				if (control_mode) {
					control(c);
					print_mode(control_mode);
					continue;
				}
				char* str_tmp = realloc(str, size + 1); // +2 para novo char e '\0'
				if (!str_tmp) {
					perror("realloc");
					free(str);
					exit(1);
				}
				str = str_tmp;

				str[size - 1] = c;
				str[size] = '\0';
				size++;
				break;
		}


		system("clear");
		printf("%s", str);
		print_mode(control_mode); // Exibe o modo depois de limpar a tela
		fflush(stdout);
	}

	free(str);
	return 0;

}

