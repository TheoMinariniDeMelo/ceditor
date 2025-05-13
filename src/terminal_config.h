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
#include <termios.h>
#define gotoxy(x,y) printf("\033[%d;%dH", (x), (y))
#define clear() system("clear");


void disable_raw_mode();

void clean();

void clear_sig(int i);

/*
 *STDIN_FILENO = 0
 *STDOUT_FILENO = 1
 *TCSANOW aplica as configurações no terminal sem esperar que o buffer seja processado
 *ECHO flag faz com que cada tecla digitada apareça no stdout
 * */
void enable_raw_mode();

void atexit_config();
typedef struct {
	int row;
	int col;
} Cursor;

unsigned short get_max_line();

unsigned short get_max_col();

void set_background();

Cursor get_cursor();

void print_char(char ch, int pX, int pY);

void put_string(char* str, size_t size);

void resize_handler(int sig);
