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
	gotoxy(get_max_line(),1); // Move para a última linha
	printf("\033[K");                  // Limpa a linha antes de escrever
	printf(is_edit_mode ? "Edit Mode" : "Control Mode");
	printf("\033[u");            // Restaura a posição original do cursor
	fflush(stdout);
}
/*
   0000 0000-0000 007F | 0xxxxxxx (
   0000 0080-0000 07FF | 110xxxxx 10xxxxxx
   0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
   0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
*/
// 10xxxxxx & 10000000
void remove_chars(unsigned char **str, size_t *size, int n) {
    if (*size <= n) {
        *size = 1;
        **str = '\0';
        return;
    }
    *size -= n;
    unsigned char *str_tmp = realloc(*str, *size);
    if (!str_tmp) {
        perror("realloc");
        free(*str);
        exit(1);
    }
    *str = str_tmp;
    (*str)[*size - 1] = '\0';
}

int utf8_char_bytes(unsigned char *str, size_t size) {
    if (size <= 1) return 0;

    int bytes = 1;
    size_t pos = size - 2;
    while (pos > 0 && (str[pos] & 0xC0) == 0x80) {
        bytes++;
        pos--;
    }

    unsigned char start = (bytes == 1) ? str[size - 1] : str[pos];
    if (start <= 0x7F) return 1;
    if ((start & 0xE0) == 0xC0) return 2;
    if ((start & 0xF0) == 0xE0) return 3;
    if ((start & 0xF8) == 0xF0) return 4;

    return bytes;
}
int main() {
	resize_handler(0);
	signal(SIGWINCH, resize_handler);
	signal(SIGINT, clear_sig);
	enable_raw_mode();
	atexit_config();
	system("clear");

	unsigned char c;
	size_t size = 1;

	unsigned char *str = malloc(size); // +1 para '\0'
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
				int n = utf8_char_bytes(str, size);
				remove_chars(&str, &size, n);
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
				unsigned char* str_tmp = realloc(str, size + 1); // +2 para novo char e '\0'
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

