/*
    Neste módulo vão ser definidas funções auxiliares à resolução do projet
*/

#include "../include/utilities.h"

char* strdup_n(const char* buf, int x) {
    char* new_str = malloc(x + 1);
    memcpy(new_str, buf, x);   // Copia os primeiros x bytes de buf para a nova string
    new_str[x] = '\0';
    return new_str;
}

void copy_args_prog(char* dest[][20], char* src[][20], int nr_p){
    for (int i = 0; i < nr_p; i++) {
        for (int j = 0; j < 20; j++) {
            strcpy(dest[i][j], src[i][j]);
        }
    }
}