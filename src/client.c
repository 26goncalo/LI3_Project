/*
Os utilizadores devem usar um programa cliente para submeter ao servidor a intenção de executar uma tarefa, dando uma
indicação da duração em milissegundos que necessitam para a mesma, e qual a tarefa (i.e., programa ou conjunto/pipeline de
programas) a executar.

Cada tarefa tem associado um identificador único que deve ser transmitido ao cliente mal o servidor recebe a mesma. O servidor
é responsável por escalonar e executar as tarefas dos utilizadores. 

Informação produzida pelas tarefas para o standard output ou standard error devem ser redirecionadas
pelo servidor para um ficheiro cujo nome corresponde ao identificador da tarefa.

Através do programa cliente, os utilizadores podem ainda consultar o servidor para saberem quais as tarefas em execução, em
espera para execução, e terminadas.
*/

// int command_write()

#include <stdio.h>
int main(){
    printf("Test");
    return 0;
}
