#include <stdio.h>

int get_buffer(FILE* file, char* buff, unsigned int buff_size, int absolute_position){
	fseek(file, absolute_position, SEEK_SET);
	fread(buff, 1, buff_size, file);
	return 0;
}

/*
 * SEEK_SET: POSIÇÃO INICIAL DO CURSOS
 * SEEK_CUR: POSIÇÃO ATUAL DO CURSOR
 * SEEK_END: POSIÇÃO FINAL DO CURSOR
 * */
#include <stdio.h>
#include <string.h>

int save_buffer(FILE* file, char* buff, unsigned int buff_size, int absolute_position) {
    long position = ftell(file);
    if (position == -1) {
        return 1;  
    }

    if (fseek(file, absolute_position, SEEK_SET) != 0) {
        return 1; 
    }

    size_t bytes_written = fwrite(buff, 1, buff_size, file);
    if (bytes_written != buff_size) {
        fseek(file, position, SEEK_SET);  

        FILE *tmp = tmpfile();
        if (tmp == NULL) {
            return 1;  
        }

        char buffer_tmp[512];
        size_t bytes_read;
        fseek(file, 0, SEEK_SET);  

        while ((bytes_read = fread(buffer_tmp, 1, sizeof(buffer_tmp), file)) > 0) {
            fwrite(buffer_tmp, 1, bytes_read, tmp);
        }

        fseek(tmp, 0, SEEK_SET);
        while ((bytes_read = fread(buffer_tmp, 1, sizeof(buffer_tmp), tmp)) > 0) {
            fwrite(buffer_tmp, 1, bytes_read, file);
        }

        fclose(tmp);
        return 1;  
    }

    fseek(file, position, SEEK_SET);
    return 0;  
}

