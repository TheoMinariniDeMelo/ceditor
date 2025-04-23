#include <signal.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
void disable_cocked_mode(){
	struct termios raw;
	tcgetattr(STDIN_FILENO, &raw);
	raw.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSAFLUSH,&raw);
}
int main(){
	disable_cocked_mode();
	char *str = "";
	size_t size = 1; 
	while(1){
		char a = getchar();
		printf("%c", a);
		char *str_tmp = malloc(sizeof(char) * (size + 1));
		str_tmp[size - 1] = a;
		str_tmp[size] = '\0';
		if(a == 'q'){
			printf("%s", str);
			fflush(stdin);
			sleep(5);
			break;
		}
	}
}
