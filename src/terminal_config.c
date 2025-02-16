#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

struct termios original_terminal;

void disable_raw_mode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal) == -1) {
        perror("tcsetattr");
    }
}
/*
 *STDIN_FILENO = 0
 *STDOUT_FILENO = 1
 *TCSANOW aplica as configurações no terminal sem esperar que o buffer seja processado
 *ECHO flag faz com que cada tecla digitada apareça no stdout
 * */
void enable_raw_mode() {
    struct termios terminal;

    // Captura as configurações do terminal e verifica erro
    if (tcgetattr(STDIN_FILENO, &original_terminal) == -1) {
        perror("tcgetattr");
        exit(1);
    }

    // Cópia das configurações para modificar
    terminal = original_terminal;

    // Registra a função de restauração ao sair do programa
    if (atexit(disable_raw_mode) != 0) {
        perror("atexit");
        exit(1);
    }

    // Desativa o modo canônico e a exibição de entrada
    terminal.c_lflag &= ~(ICANON | ECHO);

    // Aplica as novas configurações e verifica erro
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminal) == -1) {
        perror("tcsetattr");
        exit(1);
    }
}
