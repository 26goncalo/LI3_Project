#include "../include/client.h"
#include "../include/utilities.h"

#define COMMANDSIZE 100

int main(int argc, char* argv[]){
    char command[COMMANDSIZE];


    // execute time -u "prog-a [args]"
    if(argc < 4) {
        if(argc == 2){
            if(my_strcmp(argv[1],"-help") == 0){
                my_strcpy(command, "-help");
                printf("teste\n"); // neste momento usado para debug
                printf("Comando: %s\n",command);
            } else {

            }
        }
        perror("Falta de argumentos");
        return -1;
    }

    //oi
    if(mkfifo("fifo",0666) == -1) perror("Erro ao criar pipeline"); // ligar o server
    
    return 0;
}