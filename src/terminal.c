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

typedef struct{
	int row;
	int col;
} Cursor;

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
	printf("\033[45m");
	for(int i = 0; i < max_line; i++){
		for(int i = 0; i < max_col; i++){
			printf(" ");
		}
	}
	gotoxy(1,1);	
}
Cursor get_cursor(){
	clear();
	write(STDOUT_FILENO, "\033[6n", 4);	
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
