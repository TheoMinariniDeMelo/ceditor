#include <signal.h>
#include "./tty.c"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char** argv){
	resize_handler(0);
	signal(SIGWINCH, resize_handler);
	if(strlen(argv[0]) == 0) return 1;
	
	char *file_name = argv[0];

	FILE* file = fopen(file_name, "rw");

	if(file == NULL) return 1;
	
	unsigned int BUFFER_SIZE = max_col * max_line;
	char *buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));
	
	if(fgets(buffer, BUFFER_SIZE, file) == NULL){
		free(buffer);
		return 1;
	} 

}
