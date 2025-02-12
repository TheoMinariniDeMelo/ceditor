#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>


unsigned int max_line;
unsigned int max_col;

void resize_handler(int sig){
	struct winsize ws;	
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
	}
	max_line = ws.ws_row;  // Atualiza o número de linhas
	max_col = ws.ws_col;   // Atualiza o número de colunas
	
}

