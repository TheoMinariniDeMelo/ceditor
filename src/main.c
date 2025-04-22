#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include "./terminal_config.h"

// ========== UTILITÁRIOS ==========

void handle_control_mode_input(char ch) {
	if (ch == 'q') exit(0);
}

void display_mode(bool is_edit_mode) {
	printf("\033[s");                     // Salva cursor
	gotoxy(get_max_line(), 1);            // Vai para última linha
	printf("\033[K");                     // Limpa linha
	printf(is_edit_mode ? "Edit Mode" : "Control Mode");
	printf("\033[u");                     // Restaura cursor
	fflush(stdout);
}

int utf8_byte_count(unsigned char byte) {
	if (byte <= 0x7F) return 1;
	if ((byte & 0xE0) == 0xC0) return 2;
	if ((byte & 0xF0) == 0xE0) return 3;
	if ((byte & 0xF8) == 0xF0) return 4;

	fprintf(stderr, "Invalid UTF-8 byte.\n");
	exit(1);
}
int find_byte_index_by_element(int element_index, unsigned char* str, int size) {
	int byte_index = 0;
	int element_count = 0;

	FILE* file = fopen("./ceditor.log", "a+");
	if (!file) {
		perror("fopen");
		exit(1);
	}

	fprintf(file, "ELEMENT INDEX: %d\n", element_index);
	fclose(file);
	int currenty_col = 0;
	while (byte_index < size) {
		if (element_count == element_index) 
		{
			FILE* file = fopen("./ceditor.log", "a+");
			if (!file) {
				perror("fopen");
				exit(1);
			}
			fprintf(file, "\n\nELEMENT INDEX REAL: %d\n\n", byte_index);
			fclose(file);
			return byte_index - 1;
		}
		if(str[byte_index] == '\n'){
			byte_index++;
			element_count += get_max_col() - currenty_col;
			currenty_col = 0;
			continue;
		}
		else{
			currenty_col = currenty_col < get_max_col() ? currenty_col + 1: 1; 
			byte_index += utf8_byte_count(str[byte_index]);
			element_count++;
		}
	}

	return -1;
}

int get_char_byte_size(int byte_index, unsigned char *str, size_t size) {
	if (size < 1) return 0;

	int bytes = 1;
	size_t pos = byte_index;

	// Verifica bytes de continuação UTF-8
	while (pos > 0 && (str[pos] & 0xC0) == 0x80) {
		bytes++;
		pos--;
	}

	return utf8_byte_count(str[pos]);
}

int count_utf8_chars(unsigned char *str, int size) {
	int count = 0;
	for (int i = 0; i < size;) {
		i += utf8_byte_count(str[i]);
		count++;
	}
	return count;
}
void remove_utf8_chars(unsigned char **str, size_t *size, int n, int end_index) {
	int start = end_index - n + 1;
	size_t new_size = *size - n;
	unsigned char *temp = calloc(new_size, sizeof(unsigned char));
	if (!temp) {
		perror("calloc");
		exit(1);
	}

	for (int i = 0, j = 0; i < *size; i++) {
		if (i < start || i > end_index) {
			temp[j++] = (*str)[i];
		}
	}

	FILE *file = fopen("./ceditor.log", "a+");
	if (!file) {
		perror("fopen");
		exit(1);
	}

	fprintf(file, "\n=== REMOVING UTF8 CHARS: UTF-8 (%d) end-index (%d) ===\n", n, end_index);
	fwrite(*str, 1, *size, file);
	fprintf(file, "\n--- RESULT ---\n");
	fwrite(temp, 1, new_size, file);
	fprintf(file, "\n");

	fclose(file);

	free(*str);
	*str = temp;
	*size = new_size;
}

// ========== LOOP PRINCIPAL ==========

int main() {
	// Inicialização
	resize_handler(0);
	signal(SIGWINCH, resize_handler);
	signal(SIGINT, clear_sig);
	enable_raw_mode();
	atexit_config();
	system("clear");
	bool is_edit_mode = true;
	display_mode(is_edit_mode);
	int a = 0;
	size_t size = 1;
	unsigned char *str = malloc(size);
	if (!str) {
		perror("malloc");
		exit(1);
	}
	str[0] = '\0';
	unsigned char c;
	while (read(STDIN_FILENO, &c, 1) == 1) {
	//	if (a==40) break;
	//	a++;
		switch (c) {
			case 0x1B:  // ESC: alternar modo
				is_edit_mode = !is_edit_mode;
				display_mode(is_edit_mode);
				break;
			case 0x7F:  // Backspace
				if (size <= 1) break;
				FILE *file = fopen("./ceditor.log", "a+");
				Cursor cursor = get_cursor();
				int logical_index = (cursor.row - 1) * get_max_col() + cursor.col - 1;
				int byte_index = find_byte_index_by_element(logical_index, str, size);
				fprintf(file,"\n\nLOGICAL INDEX: %d AND COL_MAX: %d\n\n", logical_index, get_max_col());
				if (byte_index == -1) break;
				int n_bytes = get_char_byte_size(byte_index, str, size);
				remove_utf8_chars(&str, &size, n_bytes, byte_index);
				fclose(file);
				break;
			default:
				if (!is_edit_mode) {
					handle_control_mode_input(c);
					display_mode(is_edit_mode);
					continue;
				}

				unsigned char *tmp = realloc(str, size + 1);
				if (!tmp) {
					perror("realloc");
					free(str);
					exit(1);
				}
				str = tmp;

				str[size - 1] = c;
				str[size] = '\0';
				size++;

				break;
		}
		set_background();
		printf("%s", str);
		display_mode(is_edit_mode);
		fflush(stdout);
	}

	FILE *file = fopen("./ceditor.log", "a+");
	fprintf(file, "\n\n\n\n");
	for(int i = 0; i < size; i++){
		/*switch(str[i]){
			case '\n':
				fprintf(file ,"\033[0;32m'\\n'\033[0m\n");
				break;
			case '\0':
				fprintf(file,"\033[0;31m'\\0'\033[0m");
				break;	
			default:
				fprintf(file,"%c", str[i]);
		}*/
		fprintf(file,"%c", str[i]);
	}
	fprintf(file,"\n\n\n\rsize: %d", (int )size);
	fclose(file);
	free(str);
	return 0;
}
