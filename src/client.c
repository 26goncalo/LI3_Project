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

int main(int argc, char* argv[]){
    
    int client_to_server = open("client_to_server", O_WRONLY);

    char buffer[1024];
    int offset = 0;
    for (int i = 1; i < argc; i++) {
        strcpy(buffer + offset, argv[i]);
        offset += strlen(argv[i]);
        // Adiciona um espaço se não for o último argumento
        if (i != (argc - 1)) {
            buffer[offset] = ' ';
            offset++;
        }
    }
    if(write(client_to_server, buffer, offset) == -1){
        perror("Erro na escrita para o pipe");
        close(client_to_server);
        return -1;
    }
    close(client_to_server);

    int server_to_client = open("server_to_client", O_RDONLY);
    int bytes_read = 0;
    char buf[1000];
    while((bytes_read = read(server_to_client, buf, 1000)) > 0){
        write(1, buf, bytes_read);
    }
    close(server_to_client);
    return 0;
}