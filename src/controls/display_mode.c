#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../terminal_config.h"

void display_mode(bool is_edit_mode, char* str) {
	printf("\033[s");                     // Salva cursor

	gotoxy(get_max_line(), 1);            // Vai para última linha

	printf("\033[K");                     // Limpa linha
	if(!is_edit_mode && strlen(str) > 1){
		printf("%s", str);
	}
	else{
		printf(is_edit_mode ? "Edit Mode" : "Control Mode");
		printf("\033[u");             // Restaura a posição do cursor
	}	
	fflush(stdout);
}


