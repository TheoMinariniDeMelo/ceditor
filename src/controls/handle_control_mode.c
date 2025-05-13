#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./double_point_control.c"
#include "../utils/split_array.h"
void handle_control_mode_input(
	char* text, 
	size_t size_text, 
	FILE* file, 
	char* str, 
	size_t size) {
	
	size_t splited_arr_size;
	char** actions = split_char(text, size_text, " ", 1, &splited_arr_size);	

	if (strcmp(actions[0], "q")) exit(0);
}
