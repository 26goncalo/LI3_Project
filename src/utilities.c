/*
    Neste módulo vão ser definidas funções auxiliares à resolução do projeto,
    visto que não podemos utilizar as bibliotecas para operações sobre ficheiros
*/

// Função para comparar 2 strings
int my_strcmp(const char *str1, const char *str2) {
    while (*str1 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

// Função para calcular o comprimento de uma string
int my_strlen(const char *str){
    int length = 0;
    while(*str++) length++; // contar até ao caracter nulo
    return length;
}

// Função para copiar uma string
void my_strcpy(char *dest, const char *src) {
    while (*src) *dest++ = *src++;
    *dest = '\0'; // colocar o caratere nulo no fim da nova string
}
