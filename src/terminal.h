#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#define clear() system("clear");


unsigned short get_max_line();
unsigned short get_max_col();
void set_background();
void init();
void get_position();
void print_char(char ch, int pX, int pY);
void put_string(char* str, size_t size);
void resize_handler(int sig);
