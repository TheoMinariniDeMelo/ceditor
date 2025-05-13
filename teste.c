#include <signal.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include "./src/utils/split_array.c"
void disable_cocked_mode(){
	struct termios raw;
	tcgetattr(STDIN_FILENO, &raw);
	raw.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSAFLUSH,&raw);
}

void print_string_array(char *array[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%s\n", array[i]);
    }
}

int main(){
	/*disable_cocked_mode();
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
	}*/
	char* str = "Meu Nome e Theo Minarini De Melo";
	size_t size = strlen(str);
	size_t final_size;
	char** splited_arr = split_char(str, size, " ", 1, &final_size);
	print_string_array(splited_arr, final_size);
	return 0;
}
