#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char** split_char(char* str, size_t size, char* condition, size_t size_condition, size_t* splited_arr_size) {
    char** splited_arr = NULL;
    *splited_arr_size = 0;
    int last = 0;

    for (int i = 0; i <= (int)(size - size_condition); i++) {
        if (strncmp(str + i, condition, size_condition) == 0) {
            int segment_len = i - last;
            char* segment = malloc(segment_len + 1);
            memcpy(segment, str + last, segment_len);
            segment[segment_len] = '\0';

            splited_arr = realloc(splited_arr, sizeof(char*) * (*splited_arr_size + 1));
            splited_arr[*splited_arr_size] = segment;
            (*splited_arr_size)++;

            last = i + size_condition;
            i += size_condition - 1;
        }
    }
    if (last < size) {
        int segment_len = size - last;
        char* segment = malloc(segment_len + 1);
        memcpy(segment, str + last, segment_len);
        segment[segment_len] = '\0';

        splited_arr = realloc(splited_arr, sizeof(char*) * (*splited_arr_size + 1));
        splited_arr[*splited_arr_size] = segment;
        (*splited_arr_size)++;
    }

    return splited_arr;
}

