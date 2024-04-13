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

#include "../include/client.h"
#include "../include/utilities.h"

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("Argumentos insuficientes\n");
        return -1;
    }
    if(argc == 2){
        if(strcmp(argv[1], "status") == 0){    // Interrogar programas em execução
            //...
            return 0;
        }
        if(strcmp(argv[1], "end") == 0){     //encerrar o server
            int fd = open("server", O_WRONLY | O_APPEND);
            if (write(fd, argv[1], strlen(argv[1])) == -1){
                close(fd);
                perror("Escrita para o server");
                return -1;
            }
            close(fd);
            return 0;
        }
        printf("Argumento inválido\n");
        return -1;
    }
    if(argc < 5){
        printf("Argumentos insuficientes\n");
        return -1;        
    }
    if(strcmp(argv[1], "execute") == 0){       //execute time -u "prog-a [args]"
        int time = atoi(argv[2]);
        char flag[2];   //"-u" ou "-p"
        strcpy(flag, argv[3]);
        if(strcmp(flag, "-u") == 0){   //vai guardar o nome do programa e os seus argumentos
            int fd = open("server", O_WRONLY | O_APPEND);
            if(write(fd, argv[4], strlen(argv[4])) == -1){
                close(fd);
                perror("Escrita para o server");
                return -1;
            }
            close(fd);
            return 0;
        }
        if(strcmp(flag, "-p") == 0){
            //...
            return 0;
        }
        printf("Flag inserida errada\n");
        return -1;
    }
    printf("Argumentos inválidos\n");
    return -1;
}