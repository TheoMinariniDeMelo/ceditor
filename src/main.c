#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "./terminal.c"
int main(int argc, char** argv){
	resize_handler(0);
	signal(SIGWINCH, resize_handler);
	//init();
	printf("\n\n\n skodjsodsdosjsodjsodjsdosjdosdj krlll");
	get_position();
}
