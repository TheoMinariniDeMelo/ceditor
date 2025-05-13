#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include "file.c"
#include "./controls/handle_control_mode.c"
#include "./controls/display_mode.c"
int utf8_byte_count(char byte) {
	if (byte <= 0x7F) return 1;
	if ((byte & 0xE0) == 0xC0) return 2;
	if ((byte & 0xF0) == 0xE0) return 3;
	if ((byte & 0xF8) == 0xF0) return 4;

	fprintf(stderr, "Invalid UTF-8 byte.\n");
	exit(1);
}
int next_tab_stop(int col) {
	const int tab_width = 8;
	return col + (tab_width - ((col - 1) % tab_width));
}
int find_byte_index_by_element(int target_index, char* str, int size) {
	int byte_index = 0;
	int element_index = 1;
	int col = 1;

	while (byte_index < size) {
		if (element_index == target_index) {
			return byte_index - 1;
		}

		char ch = str[byte_index];

		if (ch == '\n') {
			int max_col = get_max_col();
			element_index += max_col - col + 1;
			col = 1;
			byte_index++;
			continue;
		}

		if (ch == '\t') {
			int max_col = get_max_col();
			int new_col = next_tab_stop(col);

			if (new_col > max_col) {
				new_col = max_col;
			}

			// Logging auxiliar
			FILE *file = fopen("./ceditor.log", "a+");
			if (file) {
				fprintf(file, "[TAB] col=%d → new_col=%d | element_index=%d, byte_index=%d, target=%d\n",
	    col, new_col, element_index, byte_index, target_index);
				fclose(file);
			}

			element_index += new_col - col ;
			col = new_col % max_col;
			if (col == 0) col = max_col;

			byte_index++;
			continue;
		}

		// UTF-8 character
		int char_len = utf8_byte_count(ch);
		byte_index += char_len;

		col = (col < get_max_col()) ? col + 1 : 1;
		element_index++;
	}

	return -1;
}
int get_char_byte_size(int byte_index, char *str, size_t size) {
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

int count_utf8_chars(char *str, int size) {
	int count = 0;
	for (int i = 0; i < size;) {
		i += utf8_byte_count(str[i]);
		count++;
	}
	return count;
}
void remove_utf8_chars(char **str, size_t *size, int n, int end_index) {
	int start = end_index - n + 1;
	size_t new_size = *size - n;
	char *temp = calloc(new_size, sizeof(unsigned char));
	if (!temp) {
		perror("calloc");
		exit(1);
	}

	for (int i = 0, j = 0; i < *size; i++) {
		if (i < start || i > end_index) {
			temp[j++] = (*str)[i];
		}
	}

	free(*str);
	*str = temp;
	*size = new_size;
}

void update(char *str, bool is_edit_mode, unsigned char* text_control_mode){
	set_background();
	printf("%s", str);
	display_mode(is_edit_mode, text_control_mode);
	fflush(stdout);

}
int main(int argc, char *argv[]) {
	resize_handler(0);
	signal(SIGWINCH, resize_handler);
	signal(SIGINT, clear_sig);
	enable_raw_mode();
	atexit_config();
	bool is_edit_mode = true;
	display_mode(is_edit_mode, (char*) "");
	char *file_path;
	if(argc > 1){
		file_path = argv[1];
	}
	FILE* file = create(file_path);
	size_t size = get_file_size(file);

	char *str = malloc(size);
	if (!str) {
		perror("malloc");
		exit(1);
	}

	str[size - 1] = '\0';
	char c;
	char* text_control_mode = NULL;
	size_t text_control_mode_size = 0;
	while (1){
		c = getchar();
		switch (c) {
			case 0x1B:  // ESC: alternar modo
				if(getchar() == '['){
					Cursor cursor = get_cursor();
					switch(getchar()){
						case 'A': 
							gotoxy(cursor.row - 1, cursor.col);
							break;
						case 'B':
							gotoxy(cursor.row + 1, cursor.col);
							break;
						case 'C':
							gotoxy(cursor.row, cursor.col + 1);
							break;
						case 'D':
							gotoxy(cursor.row, cursor.col - 1);
							break;
					}

				}else{
					is_edit_mode = !is_edit_mode;
					update(str, is_edit_mode, text_control_mode);
				}
				break;
			case 0x7F:  // Backspace
				if(!is_edit_mode){

				}
				if (size <= 1) break;
				Cursor cursor = get_cursor();
				int logical_index = (cursor.row - 1) * get_max_col() + cursor.col;
				int byte_index = find_byte_index_by_element(logical_index, str, size);
				if (byte_index == -1) break;
				int n_bytes = get_char_byte_size(byte_index, str, size);
				remove_utf8_chars(&str, &size, n_bytes, byte_index);
				update(str, is_edit_mode, text_control_mode);
				break;
			default:
				if (!is_edit_mode && c == 0x0A) {
					handle_control_mode_input(text_control_mode, text_control_mode_size, file, str, size);
					//display_mode(is_edit_mode, text_control_mode);
					continue;
				}
				else if(!is_edit_mode){

				}
				char *tmp = realloc(str, size + 1);
				if (!tmp) {
					perror("realloc");
					free(str);
					exit(1);
				}
				str = tmp;

				str[size - 1] = c;
				str[size] = '\0';
				size++;
				update(str, is_edit_mode);
				break;
		}
	}

	free(str);
	return 0;
}
