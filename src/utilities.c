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