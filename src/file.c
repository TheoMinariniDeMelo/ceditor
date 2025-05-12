#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#define PATTERN "^[a-zA-Z0-9/_-]+$"

int get_buffer(FILE* file, char* buff, unsigned int buff_size, int absolute_position){
	fseek(file, absolute_position, SEEK_SET);
	fread(buff, 1, buff_size, file);
	return 0;
}
size_t get_file_size(FILE* file){
	fseek(file, 0, SEEK_END); // seek to end of file
	size_t size = ftell(file); // get current file pointer
	fseek(file, 0, SEEK_SET); // seek back to beginning of file
	if(size == 0) return 1;
	else return size;
}
int save_file_content(char* file_path, char* content, int size){
	FILE *file = fopen(file_path, "w");
	if(file == NULL){
		fprintf(stderr,"error: open file on save file content");
		fclose(file);
		exit(1);
	}
	if(fwrite(content, sizeof(char), size, file) == -1){
		fprintf(stderr, "error: write content in file on save file content");
		fclose(file);
		exit(1);
	}
	fclose(file);
	return 0;
}
int validate_path(char *path){
	regex_t reg;
	int result;

	if (regcomp(&reg , PATTERN, REG_EXTENDED|REG_NOSUB) != 0) {
		fprintf(stderr,"erro regcomp\n");
		exit(1);
	}

	result = regexec(&reg, path, 0, NULL, 0);
	regfree(&reg);

	if (result == 0)
		return 1;
	else
		return -1;
}
/*
 * SEEK_SET: POSIÇÃO INICIAL DO CURSOS
 * SEEK_CUR: POSIÇÃO ATUAL DO CURSOR
 * SEEK_END: POSIÇÃO FINAL DO CURSOR
 * */
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
FILE* create(char* path){
	if(validate_path(path) == -1){
		fprintf(stderr, "The give path is incorrect");
		exit(1);
	};
	FILE* file = fopen(path, "a+");
	if(file == NULL){
		fprintf(stderr, "error in create file in %s", path);
		exit(1);
	}
	return file;
}
