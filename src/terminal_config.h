#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
void disable_raw_mode();
void enable_raw_mode();

/*char getch(){
	struct termios terminal;
	tcgetattr(STDIN_FILENO, &original_terminal);
	terminal = original_terminal;	
	terminal.c_lflag &= ~ (ICANON | ECHO); // aplica uma operação AND bitwise entre o lado esquerdo e o direito
	tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
	char ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &original_terminal);
	return ch;
}*/
