#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "terminal.h"

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
