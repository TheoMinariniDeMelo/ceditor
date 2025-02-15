#include <termios.h>
#include <unistd.h>
#include <stdio.h>


/*
 *STDIN_FILENO = 0
 *STDOUT_FILENO = 1
 *TCSANOW aplica as configurações no terminal sem esperar que o buffer seja processado
 * */
void disable_echo(){
	struct termios terminal;
	tcgetattr(STDIN_FILENO, &terminal);
	terminal.c_lflag &= ~ (ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
}

void enable_echo(){
	struct termios terminal;
	tcgetattr(STDIN_FILENO, &terminal);
	terminal.c_lflag &= (ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
}

char getch(){
	struct termios old_terminal, new_terminal;
	tcgetattr(STDIN_FILENO, &old_terminal);
	new_terminal = old_terminal;	
	new_terminal.c_lflag &= ~ (ICANON | ECHO); // aplica uma operação AND bitwise entre o lado esquerdo e o direito
	tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal);
	char ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal);
	return ch;
}
