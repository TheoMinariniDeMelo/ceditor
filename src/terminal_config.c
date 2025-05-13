#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define gotoxy(x,y) printf("\033[%d;%dH", (x), (y))
#define clear() system("clear");

unsigned short max_line;
unsigned short max_col;
struct termios original_terminal;

typedef struct{
	int row;
	int col;
} Cursor;


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

unsigned short get_max_line(){
	return max_line;	
}

unsigned short get_max_col(){
	return max_col;	
}

void set_background()
{
	clear();
	gotoxy(1,1);
	//printf("\033[45m");
	for(int i = 0; i < max_line; i++){
		for(int i = 0; i < max_col; i++){
			printf(" ");
		}
	}
	gotoxy(1,1);	
}
Cursor get_cursor() {
	write(STDOUT_FILENO, "\033[6n", 4);  // Solicita posição do cursor
	fflush(stdout);
	char response[32];
	char ch;
	Cursor cursor = {};
	int i = 0;

	while (i < sizeof(response) - 1) {
		if (read(STDIN_FILENO, &ch, 1) != 1) break;
		response[i++] = ch;
		if (ch == 'R') break;
	}
	response[i] = '\0';

	if (sscanf(response, "\033[%d;%dR", &(cursor.row), &(cursor.col)) != 2) {
		cursor.row = cursor.col = -1;
	}

	return cursor;
}

void print_char(char ch, int pX, int pY){
	set_background();
	gotoxy(pX, pY);
}

void put_string(char* str, size_t size){
	char arr[max_col];
	int str_position = 0;
	for(int i=0 ; i < max_line; i++){
		memcpy(arr, &str[str_position], max_col - 1);
		char* p = strchr(arr, '\n');
		if(p != NULL){
			memcpy(arr, &str[str_position], arr-p-1);
		}

		write(STDOUT_FILENO, arr, strlen(str));
		str_position += arr-p+1;
	}	

	set_background();
}
void resize_handler(int sig){
	struct winsize ws;	
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
	}
	max_line = ws.ws_row; 
	max_col = ws.ws_col; 
	if(max_col < 50 && max_line < 40){
		printf("You must resize your terminal!");
		exit(0);
	}

	set_background();	
}
