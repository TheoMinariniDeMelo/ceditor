#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include "./terminal_config.h"

// ================== FUNÇÕES DE UTILIDADE ==================

void control_mode_handler(char ch) {
	if (ch == 'q') {
		exit(0);
	}
}

void print_mode(bool is_edit_mode) {
	printf("\033[s");                     // Salva a posição do cursor
	gotoxy(get_max_line(), 1);            // Move para a última linha
	printf("\033[K");                     // Limpa a linha antes de escrever
	printf(is_edit_mode ? "Edit Mode" : "Control Mode");
	printf("\033[u");                     // Restaura a posição original do cursor
	fflush(stdout);
}

int verify_utf8(unsigned char value) {
	if (value <= 0x7F) return 1;
	else if ((value & 0xE0) == 0xC0) return 2;
	else if ((value & 0xF0) == 0xE0) return 3;
	else if ((value & 0xF8) == 0xF0) return 4;

	perror("Char has not been able to be verified!");
	exit(1);
}

int get_index_element(int element, unsigned char* str, int size) {
	int index = 0;
	int i = 0;

	while (i <= element && index < size) {
		index += verify_utf8(str[index]);
		i++;
	}

	return (i == element && index < size) ? index : -1;
}

int utf8_char_bytes(int index_element, unsigned char *str, size_t size) {
	if (size < 1) return 0;

	int bytes = 1;
	size_t pos = index_element;

	// Conta bytes de continuação (0x80 a 0xBF)
	while (pos > 0 && (str[pos] & 0xC0) == 0x80) {
		bytes++;
		pos--;
	}

	return verify_utf8(str[pos]);
}

int count_utf8_chars(unsigned char *str, int size) {
	int count = 0;
	for (int i = 0; i < size;) {
		i += verify_utf8(str[i]);
		count++;
	}
	return count;
}

void remove_chars(unsigned char **str, size_t *size, int n, int index_element) {
	int start = index_element - n + 1;
	size_t new_size = *size - n;
	unsigned char *str_tmp = calloc(new_size, sizeof(unsigned char));
	if (!str_tmp) {
		perror("calloc");
		exit(1);
	}

	for (int i = 0, j = 0; i < *size; i++) {
		if (i < start || i > index_element) {
			str_tmp[j++] = (*str)[i];
		}
	}

	free(*str);
	*str = str_tmp;
	*size = new_size;
}

// ================== LOOP PRINCIPAL ==================

int main() {
	resize_handler(0);
	signal(SIGWINCH, resize_handler);
	signal(SIGINT, clear_sig);
	enable_raw_mode();
	atexit_config();
	system("clear");

	bool is_edit_mode = true;
	print_mode(is_edit_mode);

	unsigned char c;
	size_t size = 1;
	unsigned char *str = malloc(size);
	if (!str) {
		perror("malloc");
		exit(1);
	}
	str[0] = '\0';

	while (read(STDIN_FILENO, &c, 1) == 1) {
		switch (c) {
			case 0x1B: // ESC: alternar modo
				is_edit_mode = !is_edit_mode;
				print_mode(is_edit_mode);
				break;

			case 0x7F: // Backspace
				if (size <= 1) break;

				Cursor cursor = get_cursor();
				int element = cursor.row * get_max_col() + cursor.col - 1;
				int index_element = get_index_element(element, str, size);
				if (index_element == -1) break;

				int n = utf8_char_bytes(index_element, str, size);
				remove_chars(&str, &size, n, index_element);
				break;

			default:
				if (!is_edit_mode) {
					control_mode_handler(c);
					print_mode(is_edit_mode);
					continue;
				}

				unsigned char *str_tmp = realloc(str, size + 1);
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

		set_background();
		printf("%s", str);
		print_mode(is_edit_mode);
		fflush(stdout);
	}

	free(str);
	return 0;
}

