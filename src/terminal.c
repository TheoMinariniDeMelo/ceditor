#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#define clear() system("clear");

unsigned short max_line;
unsigned short max_col;

unsigned short get_max_line(){
	return max_line;	
}

unsigned short get_max_col(){
	return max_col;	
}

void set_background()
{
	clear();
	printf("\033[45m");
	for(int i = 0; i < max_line; i++){
		for(int i = 0; i < max_col; i++){
			printf(" ");
		}
	}
}
void init(){
	set_background();
	printf("\033[H");
}
void get_position(){
	printf("\033[6n");
	fflush(stdout);
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
		printf("%s", arr);
		str_position += arr-p+1;
		puts("");
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
